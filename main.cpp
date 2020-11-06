#include <ap_int.h>
#include <hls_stream.h>
#include"header.h"

typedef ap_uint<streamlength> streamsize;
void csrmul(hls::stream<ap_uint<streamlength>>&ina,hls::stream<ap_uint<streamlength>>&inb,
		hls::stream<ap_uint<streamlength>>&incsc,hls::stream<ap_uint<streamlength>>&out)
{

streamsize Output[size][size];
streamsize B[size][size];
streamsize colverifcation[dim];
int index=0;
for (int i=0;i<size;i++)
{
#pragma HLS UNROLL factor=4
#pragma HLS PIPELINE off

	for (int j=0;j<size;j++)
	{
#pragma HLS UNROLL factor=4
#pragma HLS PIPELINE off


		Output[i][j]=0;
		B[i][j]=inb.read();
	}
}
for (int i=0; i<dim;i++)
{
#pragma HLS LOOP_FLATTEN
#pragma HLS PIPELINE off

colverifcation[i]=incsc.read();
}



for (int i=0;i<size;i++)
{
	for (int j=0;j<2;j++)
	{
#pragma HLS UNROLL factor=4

		for (int k=0;k<size;k++)
		{
#pragma HLS UNROLL factor=4
#pragma HLS PIPELINE off

			Output[i][k]+=(ina.read())*B[colverifcation[index+j]][k];
		}
	}
	index+=2;
}


for (int i=0;i<size;i++)
{

for (int j=0;j<size;j++)
{
#pragma HLS PIPELINE off

#pragma HLS UNROLL factor=4
out.write(Output[i][j]);
}

}
}

