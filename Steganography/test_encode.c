#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>
#include "decode.h"

int main(int argc, char* argv[])
{
    EncodeInfo encInfo;
    uint img_size;
    int x=check_operation_type(argv);// Check operation type
    if(x==e_encode){
        encInfo.src_image_fname=argv[2];
        char *token=strchr(argv[2],'.');
        if(token==NULL){
            fprintf(stderr,"ERROR: Invalid file name\n");
            fprintf(stderr,"Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\nDecoding: ./lsb_steg -d <.bmp_file> [output file]\n");
            return 1;
        }
        else if(strcmp(token,".bmp")){
            fprintf(stderr,"ERROR: Invalid file type\n");
            fprintf(stderr,"Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\nDecoding: ./lsb_steg -d <.bmp_file> [output file]\n");
            return 1;
        }
        if(argc>=4){
            encInfo.secret_fname=argv[3];
            token=strchr(argv[3],'.');
            if(token==NULL){
                fprintf(stderr,"ERROR: Invalid file name\n");
                fprintf(stderr,"Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\nDecoding: ./lsb_steg -d <.bmp_file> [output file]\n");
                return 1;
            }
            memcpy(encInfo.extn_secret_file,token,4*sizeof(char));
        }
        else{
            fprintf(stderr,"ERROR: No secret file provided\n");
            return 1;
        }
        if(argc==5){
            encInfo.stego_image_fname=argv[4];
            token=strchr(argv[4],'.');
            if(strcmp(token,".bmp")){
                fprintf(stderr,"ERROR: Invalid file name\n");
                fprintf(stderr,"Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\nDecoding: ./lsb_steg -d <.bmp_file> [output file]\n");
                return 1;
            }
            printf("INFO: Output file name is %s\n",encInfo.stego_image_fname);
        }
        else{
            encInfo.stego_image_fname="encoded.bmp";
            printf("INFO: Output file not mentioned. Creating %s as default\n",encInfo.stego_image_fname);
        }
        if (open_files(&encInfo) == e_failure)
        {
            printf("ERROR: %s function failed\n", "open_files" );
            return 1;
        }
        printf("INFO: opened %s\n", encInfo.secret_fname);
        if(read_and_validate_encode_args(&encInfo)==e_failure){
            return 1;
        }
        if(do_encoding(&encInfo)==e_failure){
            fprintf(stderr,"ERROR: %s function failed\n", "do_encoding");
            return 1;
        }
        else{
            printf("File %s has the encoded text in it.\n",encInfo.stego_image_fname);
        }
    }
    else if(x==e_decode){
        encInfo.src_image_fname=argv[2];
        char *token=strchr(argv[2],'.');
        if(token==NULL){
            fprintf(stderr,"ERROR: Invalid file name\n");
            fprintf(stderr,"Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\nDecoding: ./lsb_steg -d <.bmp_file> [output file]\n");
            return 1;
        }
        else if(strcmp(token,".bmp")){
            fprintf(stderr,"ERROR: Invalid file type\n");
            fprintf(stderr,"Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\nDecoding: ./lsb_steg -d <.bmp_file> [output file]\n");
            return 1;
        }
        if(argc==4){
            token=strchr(argv[3],'.');
            if(token==NULL){
                encInfo.stego_image_fname=(char *)malloc(strlen(argv[3])+4);
                for(int i=0;i<strlen(argv[3]);i++){
                    encInfo.stego_image_fname[i]=argv[3][i];
                }
            }
            else{
                encInfo.stego_image_fname=(char *)malloc(token-argv[3]+4);
                for(int i=0;i<token-argv[3];i++){
                    encInfo.stego_image_fname[i]=argv[3][i];
                }
            }
        }
        else{
            char *str="decoded";
            encInfo.stego_image_fname=(char *)malloc(strlen("decoded")+4);
            for(int i=0;i<strlen(str);i++){
                encInfo.stego_image_fname[i]=str[i];
            }
            printf("INFO: Output file not mentioned\n");
        }
        if(do_decoding(&encInfo)==e_failure){
            fprintf(stderr,"ERROR: %s function failed\n", "do_decoding");
            return 1;
        }
        else{
            fprintf(stderr,"SUCCESS: %s function completed\n", "do_decoding");
            fprintf(stderr,"File %s is the decode file.\n",encInfo.stego_image_fname);
        }
    }
    else{
        printf("ERROR: Invalid operation type\n");
        return 1;
    }
    return 0;
}
