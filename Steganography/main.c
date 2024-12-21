#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>
#include "decode.h"

int main(int argc, char* argv[])
{
    EncodeInfo encInfo;         // Structure to store information about encoding
    uint img_size;
    int x=check_operation_type(argv);// Check operation type
    if(x==e_encode){            // Encoding operation
        encInfo.src_image_fname=argv[2];        //assaigning the source image file name
        char *token=strchr(argv[2],'.');        //checking if the file is of .bmp type
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
            encInfo.secret_fname=argv[3];    //assaigning the secret file name
            token=strchr(argv[3],'.');      
            if(token==NULL){
                fprintf(stderr,"ERROR: Invalid file name\n");
                fprintf(stderr,"Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\nDecoding: ./lsb_steg -d <.bmp_file> [output file]\n");
                return 1;
            }
            memcpy(encInfo.extn_secret_file,token,4*sizeof(char));  //copying the extension type of the secret file
        }
        else{       //if the secret file name is not provided
            fprintf(stderr,"ERROR: No secret file provided\n");
            return 1;   //exit the program
        }
        if(argc==5){        //if the output file name is provided
            encInfo.stego_image_fname=argv[4];      //assaigning the output file name
            token=strchr(argv[4],'.');
            if(strcmp(token,".bmp")){               //checking if the file is of .bmp type
                fprintf(stderr,"ERROR: Invalid file name\n");
                fprintf(stderr,"Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\nDecoding: ./lsb_steg -d <.bmp_file> [output file]\n");
                return 1;               //exit the program if the file is not of .bmp type
            }
            printf("INFO: Output file name is %s\n",encInfo.stego_image_fname);
        }
        else{       //if the output file name is not provided
            encInfo.stego_image_fname="encoded.bmp";            //default output file name
            printf("INFO: Output file not mentioned. Creating %s as default\n",encInfo.stego_image_fname);
        }
        if (open_files(&encInfo) == e_failure)      //opening the files
        {
            printf("ERROR: %s function failed\n", "open_files" );
            return 1;
        }
        printf("INFO: opened %s\n", encInfo.secret_fname);
        if(read_and_validate_encode_args(&encInfo)==e_failure){     //reading and validating the arguments
            return 1;
        }
        if(do_encoding(&encInfo)==e_failure){                   //encoding the secret file into the image file
            fprintf(stderr,"ERROR: %s function failed\n", "do_encoding");       //if the encoding fails exit the program
            return 1;
        }
        else{
            printf("File %s has the encoded text in it.\n",encInfo.stego_image_fname);
        }
    }
    else if(x==e_decode){           // Decoding operation
        encInfo.src_image_fname=argv[2];    //assaigning the source image file name
        char *token=strchr(argv[2],'.');
        if(token==NULL){
            fprintf(stderr,"ERROR: Invalid file name\n");
            fprintf(stderr,"Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\nDecoding: ./lsb_steg -d <.bmp_file> [output file]\n");
            return 1;
        }
        else if(strcmp(token,".bmp")){      //checking if the file is of .bmp type
            fprintf(stderr,"ERROR: Invalid file type\n");
            fprintf(stderr,"Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\nDecoding: ./lsb_steg -d <.bmp_file> [output file]\n");
            return 1;                       //exit the program if the file is not of .bmp type
        }
        if(argc==4){    //if the output file name is provided
            token=strchr(argv[3],'.');
            if(token==NULL){    //if the output file extension type is not provided
                encInfo.stego_image_fname=(char *)malloc(strlen(argv[3])+4);
                for(int i=0;i<strlen(argv[3]);i++){
                    encInfo.stego_image_fname[i]=argv[3][i];
                }
            }
            else{               //if the output file extension type is provided
                encInfo.stego_image_fname=(char *)malloc(token-argv[3]+4);
                for(int i=0;i<token-argv[3];i++){
                    encInfo.stego_image_fname[i]=argv[3][i];
                }
            }
        }
        else{           //if the output file name is not provided
            char *str="decoded";    //default output file name
            encInfo.stego_image_fname=(char *)malloc(strlen("decoded")+4);
            for(int i=0;i<strlen(str);i++){
                encInfo.stego_image_fname[i]=str[i];
            }
            printf("INFO: Output file not mentioned\n");
        }
        if(do_decoding(&encInfo)==e_failure){               //decoding the secret file from the image file
            fprintf(stderr,"ERROR: %s function failed\n", "do_decoding");
            return 1;           //exit the program if the decoding fails
        }
        else{                       //if the decoding is successful
            fprintf(stderr,"SUCCESS: %s function completed\n", "do_decoding");
            fprintf(stderr,"File %s is the decode file.\n",encInfo.stego_image_fname);
        }
    }
    else{           //if the operation type is invalid
        printf("ERROR: Invalid operation type\n");
        return 1;
    }
    return 0;
}
