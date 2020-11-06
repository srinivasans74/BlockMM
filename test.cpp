#include<iostream>
#include <ap_int.h>
#include <hls_stream.h>
#include<string.h>
using namespace std;
#include <time.h>    // for Windows APIs
#include <chrono>
#include <fstream>
#include"header.h"


typedef ap_uint<streamlength> streamsize;
double cpu_time_used;
clock_t t1, t2;

void csrmul(hls::stream<ap_uint<streamlength>>&ina,hls::stream<ap_uint<streamlength>>&inb,
		hls::stream<ap_uint<streamlength>>&incsc,hls::stream<ap_uint<streamlength>>&out);
int main()
{

int B[size][size],A[size][size];
int Output[size][size];
hls::stream<ap_uint<16>>csc("Column pointer");
hls::stream<ap_uint<16>>matrixa("Matrix a");
hls::stream<ap_uint<16>>matrixb("Matrix B");
hls::stream<ap_uint<16>>outmatr("Out Matrix");
int pp=0,k,q,jj=0;
int count3;
int row_countmetric;
int row_w;
int index=0;
int countsparseB=0;
int kk=0;
int index5=0;
int row_weight1[size];

ap_uint<16>Final[size][size];



for (int i=0;i<size;i++)
{
	for (int j=0;j<size;j++)
	{
		if (i==j)
		{
			B[i][j]=1;
			Output[i][j]=0;
			//std::cout<<"B"<<"["<<i<<"]"<<"["<<j<<"]"<<"="<<B[i][j]<<"\n";

		}
		else
		{
			B[i][j]=0;
			Output[i][j]=0;
			//std::cout<<"B"<<"["<<i<<"]"<<"["<<j<<"]"<<"="<<B[i][j]<<"\n";
		}
	}
}


ofstream myfile1 ("Input.txt");


for (int i=0;i<size;i++)
{

	for (int j=0,row_countmetric=0;j<size;j++)
	{

		if(row_countmetric<2)
		{
			A[i][j]=1;
			//std::cout<<"A"<<"["<<i<<"]"<<"["<<j<<"]"<<"="<<A[i][j]<<"\n";
			++pp;
			myfile1<<"Output"<<"["<<i<<"]"<<"["<<j<<"]"<<"="<<A[i][j]<<"\n";
			row_countmetric++;
		}
		else
		{
			A[i][j]=0;
			//std::cout<<"A"<<"["<<i<<"]"<<"["<<j<<"]"<<"="<<A[i][j]<<"\n";
			myfile1<<"Output"<<"["<<i<<"]"<<"["<<j<<"]"<<"="<<A[i][j]<<"\n";
		}
	}
}
myfile1.close();
std::cout<<"Row_weight ="<<pp;
int val[pp];
int row_verifcation[pp];
int col_verifcation[pp];
int CSCA[pp*size];
int CSCB[pp*size];
for (int i=0;i<size;i++)
{
	count3=0;
	for (int j=0; j<size ;j++)
	{
		if (A[i][j]!=0)
		{

			count3++;
		}
	}
	row_weight1[i]=count3;
}


for (k=0;k<size;k++)
{
	for (q=0;q<size;q++)
	{
		if (A[k][q]!=0)
		{
			val[jj]=A[k][q];
			row_verifcation[jj]=k;
			col_verifcation[jj]=q;
			jj++;
		}
	}

}

#ifdef DEBUG

for (int i=0 ;i<pp;i++)
{
std::cout<<"col_ptrverif"<<"["<<i<<"]"<<"="<<col_verifcation[i]<<"\n";

}
for (int i=0 ;i<pp;i++)
{
std::cout<<"row_ptrverif"<<"["<<i<<"]"<<"="<<row_verifcation[i]<<"\n";

}
for (int i=0 ;i<size;i++)
{
std::cout<<"row_weightverif"<<"["<<i<<"]"<<"="<<row_weight1[i]<<"\n";

}
#endif



t1 = clock();
auto start = std::chrono::high_resolution_clock::now();
for (int i=0;i<size;i++)
{
	row_w=row_weight1[i];
	for (int j=0;j<row_w;j++)
	{

		for (int k=0;k<size;k++)
		{

			Output[i][k]+=(val[index+j]*B[col_verifcation[index+j]][k]);
		}
	}
	index+=row_w;
}


t2=clock();
auto finish = std::chrono::high_resolution_clock::now();
std::chrono::duration<long double> elapsed = finish - start;
cpu_time_used = (long double) (t2 - t1) / CLOCKS_PER_SEC;
std::cout<<"fun() took "<<cpu_time_used<<"seconds to execute \n";
std::cout << "Elapsed time: " << elapsed.count() << " s\n";



for (int i=0;i<pp;i++)
{
	for (int k=0;k<size;k++)
	{
		CSCA[kk]=val[i];
		kk++;
	}
}

#ifdef DEBUG
for (int k=0; k<pp*size;k++)
{
std::cout<<"CSC"<<"["<<k<<"]"<<"="<<CSCA[k]<<"\n";


}
#endif

for (int k=0; k<pp*size;k++)
{
matrixa.write(CSCA[k]);


}


ofstream myfile ("Output.txt");


for (int i=0;i<size;i++)
{
	for (int j=0;j<size;j++)
	{
	//	std::cout<<"A"<<"["<<i<<"]"<<"["<<j<<"]"<<"="<<Output[i][j]<<"\n";
		myfile<<"Output"<<"["<<i<<"]"<<"["<<j<<"]"<<"="<<Output[i][j]<<"\n";
	}
}
myfile.close();

for (int i=0;i<pp;i++)
{

csc.write(col_verifcation[i]);

}

for (int i=0;i<size;i++)
{
	for (int j=0; j<size;j++)
	{
		matrixb.write(B[i][j]);

	}
}


csrmul(matrixa,matrixb,csc,outmatr);
ofstream myfilestream ("Outputsteam.txt");

for(int i=0;i<size;i++)
		{
	for (int j=0;j<size;j++)
	{
		Final[i][j]=outmatr.read();
		myfilestream<<"Output"<<"["<<i<<"]"<<"["<<j<<"]"<<"="<<Final[i][j]<<"\n";
	}
		}

myfilestream.close();

for (int i=0; i<size;i++)
{
	for (int j=0;j<size;j++)
	{
		if (Output[i][j]!=Final[i][j])
		{
			std::cout<<"Test Failed";
			return 0;
		}
	}
}
int null =(size*size)-pp;
std::cout<<"Sparsity ="<<((null/(size*size)))<<"\n";
std::cout<<"Test Passed"<<"\n";
std::cout<<"No of elements="<<pp<<"\n";

return 0;
}
