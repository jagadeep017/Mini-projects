#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>
#include "common.h"


//functionj to decode magic string
Status decoding_ms(EncodeInfo *encInfo){
    encInfo->image_data[0]=0;       //char to accumulate data bit-by-bit
    fseek(encInfo->fptr_src_image, 54, SEEK_SET);//skiping the header data
    char temp[strlen(MAGIC_STRING)+1];
    printf("INFO: Decoding magic string\n");
    for(int i=0;i<strlen(MAGIC_STRING);i++){
         for(int i=0;i<8;i++){
            fread(encInfo->secret_data,sizeof(char),sizeof(char),encInfo->fptr_src_image);
            encInfo->image_data[0]|=((encInfo->secret_data[0]&1)<<i);
        }
        temp[i]=encInfo->image_data[0];
        encInfo->image_data[0]=0;
    }
    temp[strlen(MAGIC_STRING)]='\0';
    if(strcmp(temp,MAGIC_STRING)!=0){
        fprintf(stderr,"ERROR: MAGIC STRING mismatch,Not a valid stego file\n");
        return e_failure;
    }
    printf("INFO: Done\n\n");
    return e_success;
}

//Function to open source image
Status open_src_file(EncodeInfo *encInfo){
    printf("INFO: opeing source image %s\n",encInfo->src_image_fname);
    encInfo->fptr_src_image=fopen(encInfo->src_image_fname,"r");
    if(encInfo->fptr_src_image==NULL){
        perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
    	return e_failure;
    }
    printf("INFO: Done file opened\n\n");
    return e_success;
}

//Function to open output image
Status open_dst_file(EncodeInfo *encInfo){
    printf("INFO: opeing output file %s\n",encInfo->stego_image_fname);
    encInfo->fptr_stego_image=fopen(encInfo->stego_image_fname,"w");
    if(encInfo->fptr_stego_image==NULL){
        perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
    	return e_failure;
    }
    printf("INFO: Done file opened\n\n");
    return e_success;
}

//decoding secret file extension
Status decode_sf_extn(EncodeInfo *encInfo){
    long int length=0;
    printf("INFO: Decoding secret file extension\n");
    for(int i=0;i<8;i++){
        fread(encInfo->secret_data,sizeof(char),sizeof(char),encInfo->fptr_src_image);
        encInfo->image_data[0]|=((encInfo->secret_data[0]&1)<<i);
    }
    length=encInfo->image_data[0];
    encInfo->image_data[0]=0;
    for(int i=0;i<length;i++){
        for(int j=0;j<8;j++){
            fread(encInfo->secret_data,sizeof(char),sizeof(char),encInfo->fptr_src_image);
            encInfo->image_data[0]|=((encInfo->secret_data[0]&1)<<j);
        }
        encInfo->extn_secret_file[i]=encInfo->image_data[0];
        encInfo->image_data[0]=0;
    }
    printf("INFO: Done\n\n");
    int temp1=strlen(encInfo->stego_image_fname);
    for(int i=0;i<4;i++){
        encInfo->stego_image_fname[i+temp1]=encInfo->extn_secret_file[i];
    }
    return e_success;
}

//Function to decode size and data from file
Status decode_size_and_data(EncodeInfo *encInfo){
    long int length=0;
    printf("INFO: Decoding secret file size\n");
    for(int i=0;i<64;i++){
        fread(encInfo->image_data,sizeof(char),sizeof(char),encInfo->fptr_src_image);
        length|=((encInfo->image_data[0]&1)<<i);
    }
    encInfo->image_data[0]=0;
    encInfo->size_secret_file=length;
    printf("INFO: Done\n\n");
    printf("INFO: Decoding secret file data\n");
    for(int j=0;j<encInfo->size_secret_file;j++){
        for(int i=0;i<8;i++){
            fread(encInfo->secret_data,sizeof(char),sizeof(char),encInfo->fptr_src_image);
            encInfo->image_data[0]|=((encInfo->secret_data[0]&1)<<i);
        }
        fputc(encInfo->image_data[0],encInfo->fptr_stego_image);
        encInfo->image_data[0]=0;
    }
    printf("INFO: Done\n\n");
    return e_success;
}

//Function to decode data from file
Status do_decoding(EncodeInfo *encInfo){
    if(open_src_file(encInfo)==e_failure){
        return e_failure;
    }
    printf("INFO: #### Decoding started ####\n");
    if(decoding_ms(encInfo)==e_failure){
        return e_failure;
    }
    if(decode_sf_extn(encInfo)==e_failure){
        return e_failure;
    }
    if(open_dst_file(encInfo)==e_failure){
        return e_failure;
    }
    if(decode_size_and_data(encInfo)==e_failure){
        return e_failure;
    }
    printf("INFO: #### Decoding done successfully ####\n");
    return e_success;
}