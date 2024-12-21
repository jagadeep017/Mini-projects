#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>
#include "common.h"

/* Function Definitions */


/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */


uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

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
    // Stego Image file or decoded text file
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

/* Get File size */
uint get_file_size(FILE *fptr){
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

/* Check if secret file can be encoded into source image */
Status check_capacity(EncodeInfo *encInfo){
    printf("INFO: checking %s size\n",encInfo->secret_fname);
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret); 
    if(encInfo->size_secret_file==0){
        fprintf(stderr,"ERROR: Secret file is empty\n");
        return e_failure;
    }
    else{
        printf("INFO: Done ,%s is not empty, size = %ld\n",encInfo->secret_fname,encInfo->size_secret_file);
    }
    printf("INFO: checking for %s capacity to handle %s\n",encInfo->src_image_fname,encInfo->secret_fname);
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image)-54;
    if((encInfo->size_secret_file+strlen(MAGIC_STRING)+strlen(encInfo->extn_secret_file)+1+8)*8>encInfo->image_capacity){
        fprintf(stderr,"ERROR: Secret file size is too large to enocode into the source image\n");
        return e_failure;
    }
    else{
        printf("INFO: Done ,%s can be encoded into %s\n",encInfo->secret_fname,encInfo->src_image_fname);
    }
    return e_success;
}

/* Copy bmp header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image){
    printf("INFO: copying bmp header\n");
    fseek(fptr_src_image, 0, SEEK_SET);
    for(int i=0;i<54;i++){//copying the header info from source image to stego image
        fputc(fgetc(fptr_src_image),fptr_dest_image);
    }
    printf("INFO: copied bmp header\n");
    return e_success;
}

/* Store Magic String */
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

/* Encode secret file extn */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo){
    printf("INFO: encoding secret file extension \n");
    char temp=strlen(file_extn);
    for(int i=0;i<8;i++){
        fread(encInfo->image_data,sizeof(char),sizeof(char),encInfo->fptr_src_image);
        encInfo->image_data[0]=(encInfo->image_data[0]&(~1))|(1&temp);
        fwrite(encInfo->image_data,sizeof(char),sizeof(char),encInfo->fptr_stego_image);
        temp>>=1;
    }
    temp=strlen(file_extn);
    for(int i=0;i<temp;i++){
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

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo){
    printf("INFO: encoding secret file size \n");
    for(int i=0;i<64;i++){
        fread(encInfo->image_data,sizeof(char),sizeof(char),encInfo->fptr_src_image);
        encInfo->image_data[0]=(encInfo->image_data[0]&(~1))|(1&file_size);
        fwrite(encInfo->image_data,sizeof(char),sizeof(char),encInfo->fptr_stego_image);
        file_size>>=1;
    }
    return e_success;   
}


/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image){
    char temp;
    for(int j=0;j<8;j++){
        fread(&temp,sizeof(char),sizeof(char),fptr_src_image);
        temp=(temp&(~1))|(1&*data);
        fwrite(&temp,sizeof(char),sizeof(char),fptr_stego_image);
        *data>>=1;
    }
}

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest){
    printf("INFO: copying remaining image data\n");
    char ch;
    while(!feof(fptr_src)){
        fread(&ch,sizeof(char),sizeof(char),fptr_src);
        fwrite(&ch,sizeof(char),sizeof(char),fptr_dest);
    }
    return e_success;
}
/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo){////////////////
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
    if(check_capacity(encInfo)==e_failure){
        return e_failure;
    }
    copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image);
    fseek(encInfo->fptr_secret, 0, SEEK_SET);
    encode_magic_string(MAGIC_STRING,encInfo);
    encode_secret_file_extn(encInfo->extn_secret_file,encInfo);
    encode_secret_file_size(encInfo->size_secret_file,encInfo);
    encode_secret_file_data(encInfo);

    
    printf("INFO: #### Encoding done successfully ####\n");
    return e_success;
}

// Check operation type
OperationType check_operation_type(char *argv[]){
    if(argv[1]==NULL){
        printf("\nEncoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\nDecoding: ./lsb_steg -d <.bmp_file> [output file]\n");
        return e_unsupported;
    }
    if (strcmp(argv[1], "-e") == 0){
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0){
        return e_decode;
    }
    else{ 
        printf("\nEncoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\nDecoding: ./lsb_steg -d <.bmp_file> [output file]\n");
        return e_unsupported;
    }
}

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(EncodeInfo *encInfo){
    short int temp;
    fread(&temp,sizeof(short int),1,encInfo->fptr_src_image);
    if(temp!=0x4d42){
        fprintf(stderr,"ERROR: Not a valid bmp file\n");
        return e_failure;
    }
    fseek(encInfo->fptr_secret, 0, SEEK_END);
    return e_success;
}