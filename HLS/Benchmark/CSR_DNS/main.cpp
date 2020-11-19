#include<hls_stream.h>
#include<ap_int.h>
#define size 560
#if size==730
#define lim size*63024
#elif size==560
#define lim size*8736
#endif

void top(hls::stream<ap_uint<32>>&ina,hls::stream<ap_uint<32>>&matrixbcsc,
		hls::stream<ap_uint<32>>&out)

{

for (int i=0;i<lim;i++)
{
#pragma HLS Pipeline II=1
			out.write((ina.read())*(matrixbcsc.read()));
				}


}
