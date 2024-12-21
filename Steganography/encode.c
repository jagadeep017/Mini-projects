#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>
#include "common.h"

//function to get image size
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);

    // Return image capacity
    return width * height * 3;
}

//function to open files
Status open_files(EncodeInfo *encInfo)
{
    printf("INFO: opeing required files\n");

    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    printf("INFO: opened %s\n", encInfo->src_image_fname);

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }
    printf("INFO: opened %s\n", encInfo->stego_image_fname);
    return e_success;
}

//functon to get file size
uint get_file_size(FILE *fptr){
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

//function to Check if secret file can be encoded into source image
Status check_capacity(EncodeInfo *encInfo){
    printf("INFO: checking %s size\n",encInfo->secret_fname);
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);  //get size of secret file
    if(encInfo->size_secret_file==0){               //check if secret file is empty
        fprintf(stderr,"ERROR: Secret file is empty\n");
        return e_failure;
    }
    else{
        printf("INFO: Done ,%s is not empty, size = %ld\n",encInfo->secret_fname,encInfo->size_secret_file);
    }
    printf("INFO: checking for %s capacity to handle %s\n",encInfo->src_image_fname,encInfo->secret_fname);
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image)-54;
    //check if secret file can be encoded into source image
    if((encInfo->size_secret_file+strlen(MAGIC_STRING)+strlen(encInfo->extn_secret_file)+1+8)*8>encInfo->image_capacity){
        fprintf(stderr,"ERROR: Secret file size is too large to enocode into the source image\n");
        return e_failure;           //return failure if secret file size is too large
    }
    else{
        printf("INFO: Done ,%s can be encoded into %s\n",encInfo->secret_fname,encInfo->src_image_fname);
    }
    return e_success;
}

//Copy bmp header data from src to stego image
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image){
    printf("INFO: copying bmp header\n");
    fseek(fptr_src_image, 0, SEEK_SET);
    for(int i=0;i<54;i++){//copying the header info byte by byte from source image to stego image
        fputc(fgetc(fptr_src_image),fptr_dest_image);
    }
    printf("INFO: copied bmp header\n");
    return e_success;
}

//function to encode magic string into the output image
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo){
    printf("INFO: encoding magic string \n");
    for(int i=0;i<strlen(MAGIC_STRING);i++){
        encInfo->secret_data[0]=magic_string[i];
        for(int j=0;j<8;j++){
            fread(encInfo->image_data,sizeof(char),sizeof(char),encInfo->fptr_src_image);
            encInfo->image_data[0]=(encInfo->image_data[0]&(~1))|(1&encInfo->secret_data[0]);
            fwrite(encInfo->image_data,sizeof(char),sizeof(char),encInfo->fptr_stego_image);
            encInfo->secret_data[0]>>=1;
        }
    }
    return e_success;
}

//function to encode secret file extension into the output image
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo){
    printf("INFO: encoding secret file extension \n");
    char temp=strlen(file_extn);
    for(int i=0;i<8;i++){       //encoding the length of the file extension
        fread(encInfo->image_data,sizeof(char),sizeof(char),encInfo->fptr_src_image);
        encInfo->image_data[0]=(encInfo->image_data[0]&(~1))|(1&temp);
        fwrite(encInfo->image_data,sizeof(char),sizeof(char),encInfo->fptr_stego_image);
        temp>>=1;
    }
    temp=strlen(file_extn);
    for(int i=0;i<temp;i++){        //encoding the file extension
        encInfo->secret_data[0]=file_extn[i];
        for(int j=0;j<8;j++){
            fread(encInfo->image_data,sizeof(char),sizeof(char),encInfo->fptr_src_image);
            encInfo->image_data[0]=(encInfo->image_data[0]&(~1))|(1&encInfo->secret_data[0]);
            fwrite(encInfo->image_data,sizeof(char),sizeof(char),encInfo->fptr_stego_image);
            encInfo->secret_data[0]>>=1;
        }
    }
    return e_success;
}

//function to encode secret file size into the output image
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo){
    printf("INFO: encoding secret file size \n");
    for(int i=0;i<64;i++){          //encoding the size of the secret file into the image
        fread(encInfo->image_data,sizeof(char),sizeof(char),encInfo->fptr_src_image);
        encInfo->image_data[0]=(encInfo->image_data[0]&(~1))|(1&file_size);
        fwrite(encInfo->image_data,sizeof(char),sizeof(char),encInfo->fptr_stego_image);
        file_size>>=1;
    }
    return e_success;   
}

//function to encode data into image
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image){
    char temp;
    for(int j=0;j<8;j++){    //encoding 1 byte of data into 8 bytes of image
        fread(&temp,sizeof(char),sizeof(char),fptr_src_image);
        temp=(temp&(~1))|(1&*data);
        fwrite(&temp,sizeof(char),sizeof(char),fptr_stego_image);
        *data>>=1;
    }
}

//function to copy remaining image data
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest){
    printf("INFO: copying remaining image data\n");
    char ch;
    while(!feof(fptr_src)){
        fread(&ch,sizeof(char),sizeof(char),fptr_src);
        fwrite(&ch,sizeof(char),sizeof(char),fptr_dest);
    }
    return e_success;
}

//function to encode secret file data into the output image
Status encode_secret_file_data(EncodeInfo *encInfo){
    printf("INFO: Encoding the text into lsb's\n");
    int i=0;
    while(i<encInfo->size_secret_file){
        fread(encInfo->secret_data,sizeof(char),sizeof(char),encInfo->fptr_secret);
        encode_data_to_image(encInfo->secret_data,8,encInfo->fptr_src_image,encInfo->fptr_stego_image);
        i++;
    }
    copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}

//Function to encode data into file
Status do_encoding(EncodeInfo *encInfo){
    printf("INFO: #### Encoding started ####\n");
    if(check_capacity(encInfo)==e_failure){         //check if secret file can be encoded into source image
        return e_failure;
    }
    copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image);         //copy bmp header from source image to stego image
    fseek(encInfo->fptr_secret, 0, SEEK_SET);                                   //set the file pointer to the start of the secret file
    encode_magic_string(MAGIC_STRING,encInfo);                                  //encode magic string into the output image
    encode_secret_file_extn(encInfo->extn_secret_file,encInfo);                 //encode secret file extension into the output image
    encode_secret_file_size(encInfo->size_secret_file,encInfo);                 //encode secret file size into the output image
    encode_secret_file_data(encInfo);                                           //encode secret file data into the output image
    fclose(encInfo->fptr_src_image);                                            //close the source image file
    printf("INFO: #### Encoding done successfully ####\n");
    return e_success;
}

// Check operation type
OperationType check_operation_type(char *argv[]){
    if(argv[1]==NULL){  //check if the operation type is given
        printf("\nEncoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\nDecoding: ./lsb_steg -d <.bmp_file> [output file]\n");
        return e_unsupported;
    }
    if (strcmp(argv[1], "-e") == 0){    //check if the operation type is encode
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0){   //check if the operation type is decode
        return e_decode;
    }
    else{                           //return unsupported if the operation type is not supported
        printf("\nEncoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\nDecoding: ./lsb_steg -d <.bmp_file> [output file]\n");
        return e_unsupported;
    }
}

//function to read and validate encode arguments
Status read_and_validate_encode_args(EncodeInfo *encInfo){
    short int temp;
    fread(&temp,sizeof(short int),1,encInfo->fptr_src_image);
    if(temp!=0x4d42){               //check if the file is a valid bmp file
        fprintf(stderr,"ERROR: Not a valid bmp file\n");
        return e_failure;
    }
    fseek(encInfo->fptr_secret, 0, SEEK_END);
    return e_success;
}