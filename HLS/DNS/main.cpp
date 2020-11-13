#include<ap_int.h>
#include<iostream>
#include<hls_stream.h>
#include "header.h"


void DNS(hls::stream<ap_uint<streamlength>>&a,hls::stream<ap_uint<streamlength>>&b,
		hls::stream<ap_uint<streamlength>>&c)

{


	for (int i=0;i<streamweight;i++)
	{
		c.write((a.read())*(b.read()));

	}

}
