#include <ap_int.h>
#include <hls_stream.h>
#include"header.h"

typedef ap_uint<streamlength> streamsize;
void csrmul(hls::stream<ap_uint<streamlength>>&ina,hls::stream<ap_uint<streamlength>>&matrixbcsc,
		hls::stream<ap_uint<streamlength>>&out)
{
#pragma HLS INTERFACE axis port=ina register_mode=both register
#pragma HLS INTERFACE axis port=matrixbcsc register_mode=both register
#pragma HLS INTERFACE axis port=out register_mode=both register


	csrmul_label3:for (int i=0;i<dim*size;i++)
	{
#pragma HLS PIPELINE
			out.write((ina.read())*(matrixbcsc.read()));
				}


	}


