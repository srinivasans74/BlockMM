#include <ap_int.h>
#include <hls_stream.h>
#include"header.h"

typedef ap_uint<streamlength> streamsize;
void csrmul(hls::stream<ap_uint<streamlength>>&ina,hls::stream<ap_uint<streamlength>>&matrixbcsc,
		hls::stream<ap_uint<streamlength>>&out)
{

	streamsize Output[size][size];
	csrmul_label1:for (int i=0;i<size;i++)
	{
	#pragma HLS UNROLL
		csrmul_label2:for (int j=0;j<size;j++)
		{
	#pragma HLS UNROLL
			Output[i][j]=0;
		}
	}

	csrmul_label3:for (int i=0;i<size;i++)
	{
		csrmul_label4:for (int j=0;j<2;j++)
		{
	#pragma HLS UNROLL factor=4

			for (int k=0;k<size;k++)
			{
	#pragma HLS UNROLL factor=4
	#pragma HLS PIPELINE off

				Output[i][k]+=(ina.read())*(matrixbcsc.read());
			}
		}
	}




	csrmul_label5:for (int i=0;i<size;i++)
	{

	for (int j=0;j<size;j++)
	{
	#pragma HLS PIPELINE off

	#pragma HLS UNROLL factor=4
	out.write(Output[i][j]);
	}

	}
}

