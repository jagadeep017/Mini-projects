#ifndef DECODE_H
#define DECODE_H

#include "types.h"

/* Perform the decoding */
Status do_decoding(EncodeInfo *encInfo);//done

//functionj to decode magic string
Status decoding_ms(EncodeInfo *encInfo);

//Function to open source image
Status open_src_file(EncodeInfo *encInfo);

//Function to open output image
Status open_dst_file(EncodeInfo *encInfo);

//decoding secret file extension
Status decode_sf_extn(EncodeInfo *encInfo);

//Function to decode size and data from file
Status decode_size_and_data(EncodeInfo *encInfo);


#endif