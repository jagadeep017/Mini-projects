Steganography Project

This project implements a simple steganography tool that can encode and decode secret messages within BMP image files.


Files

common.h: Contains common definitions used in the project.

decode.c: Implementation of the decoding functionality.

decode.h: Header file for decode.c.

encode.c: Implementation of the encoding functionality.

encode.h: Header file for encode.c.

main.c: Main entry point for the steganography tool.

makefile: Makefile to build the project.

secret.txt: Example secret message to be encoded.

types.h: Contains user-defined types used in the project.


Usage

To build the project, navigate to the Steganography directory and run: make

To encode a secret file into a BMP image:

./a.out -e <source_image.bmp> <secret.txt> [output_image.bmp]

To decode a secret message from a BMP image:

./a.out -d <encoded_image.bmp> [output_file]
