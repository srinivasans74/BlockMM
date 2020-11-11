#include<iostream>
#include <ap_int.h>
#include <hls_stream.h>
#include<string.h>
using namespace std;
#include <time.h>    // for Windows APIs
#include <chrono>
#include <fstream>
#include"header.h"

clock_t t1, t2;
double cpu_time_used;
int B[size][size],A[size][size];
int Output[size][size];
ap_uint<5>Final[size][size];
int val[size*row_kernelweight];
int row_verifcation[size*row_kernelweight];
int col_verifcation[size*row_kernelweight];
int row_weight1[size];
int CSCA[row_kernelweight*size*size];
int CSCB[row_kernelweight*size*size];
void csrmul(hls::stream<ap_uint<5>>&ina,hls::stream<ap_uint<5>>&matrixbcsc,
		hls::stream<ap_uint<5>>&out);

int main()
{

hls::stream<ap_uint<5>>matrixa("Matrix a");
hls::stream<ap_uint<5>>matrixbcsc("Matrix B CSC");
hls::stream<ap_uint<5>>outmatr("Out Matrix");
int pp=0,k,q,z,jj=0;
int count3;
int row_countmetric;
int row_w;
int index=0;
int countsparseB=0;
int kk=0;
int index5=0;

for (int i=0;i<size;i++)
{
	for (int j=0;j<size;j++)
	{
		if (i==j)
		{
			B[i][j]=1;
			Output[i][j]=0;
		}
		else
		{
			B[i][j]=0;
			Output[i][j]=0;
		}
	}
}
ofstream myfile1 ("Input.txt");

for (int i=0;i<size;i++)
{
	for (int j=0,row_countmetric=0;j<size;j++)
	{
		if(row_countmetric<row_kernelweight)
		{
			A[i][j]=1;
			++pp;
			myfile1<<"Output"<<"["<<i<<"]"<<"["<<j<<"]"<<"="<<A[i][j]<<"\n";
			row_countmetric++;
		}
		else
		{
			A[i][j]=0;
			myfile1<<"Output"<<"["<<i<<"]"<<"["<<j<<"]"<<"="<<A[i][j]<<"\n";
		}
	}
}
myfile1.close();
std::cout<<"Row_weight ="<<pp<<"\n";


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


ofstream col_ptrver("Col_ptr.txt");
ofstream row_ptrver("Col_ptr.txt");
ofstream row_weightver("Col_ptr.txt");


for (int i=0 ;i<pp;i++)
{
col_ptrver<<"col_ptrverif"<<"["<<i<<"]"<<"="<<col_verifcation[i]<<"\n";

}
col_ptrver.close();
for (int i=0 ;i<pp;i++)
{
row_ptrver<<"row_ptrverif"<<"["<<i<<"]"<<"="<<row_verifcation[i]<<"\n";

}
row_ptrver.close();
for (int i=0 ;i<size;i++)
{
row_weightver<<"row_weightverif"<<"["<<i<<"]"<<"="<<row_weight1[i]<<"\n";
}
row_weightver.close();


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

for (int i=0;i<size;i++)
{
	for (int j=0;j<size;j++)
	{

		if (Output[i][j]!=A[i][j])
		{
			std::cout<<"CPU RESULT Failed"<<"\n";
		}
	}
}

for (int i=0;i<pp;i++)
{
	for (int k=0;k<size;k++)
	{
		CSCA[kk]=val[i];
		kk++;
	}
}

int qqq=0;
for (int i=0;i<pp;i++)
{
	for (int k=0;k<size;k++)
	{
		CSCB[qqq]=B[col_verifcation[i]][k];
		qqq++;
	}
}

ofstream CSCA1 ("CSCA.txt");
ofstream CSCB1 ("CSCB.txt");

for (int k=0; k<pp*size;k++)
{
CSCA1<<"CSCA"<<"["<<k<<"]"<<"="<<CSCA[k]<<"\n";
}
CSCA1.close();
for (int k=0; k<pp*size;k++)
{
CSCB1<<"CSCB"<<"["<<k<<"]"<<"="<<CSCB[k]<<"\n";
}
CSCB1.close();



for (int k=0; k<pp*size;k++)
{
matrixa.write(CSCA[k]);
matrixbcsc.write(CSCB[k]);
}


ofstream myfile ("Output.txt");


for (int i=0;i<size;i++)
{
	for (int j=0;j<size;j++)
	{
		myfile<<"Output"<<"["<<i<<"]"<<"["<<j<<"]"<<"="<<Output[i][j]<<"\n";
	}
}
myfile.close();



csrmul(matrixa,matrixbcsc,outmatr);
ofstream myfilestream ("Outputsteam.txt");

for (int i=0; i<size;i++)
{
	for (int j=0; j<size;j++)
	{
		Final[i][j]=0;

	}
}





/***
 *
 * The code below has to be modified according to the row_weight and Matrix dimmension.
 * The main objective below is row-accumulation task
 * *
 */

#if row_kernelweight ==4
for(int i=0;i<size;i++)
{
	 z=0;
	for (int j=0;j<row_kernelweight*size;j++)
	{
		if (j<size)
		{
			Final[i][j]+=outmatr.read();
			z++;
		}
		else if(j<2*size)
		{
			Final[i][j-z]+=outmatr.read();
		}
		else if (j<3*size)
		{
			Final[i][j-z-size]+=outmatr.read();
		}
		else
		{
			Final[i][j-z-(size*2)]+=outmatr.read();

		}
	}}



#elif row_kernelweight==80
for(int i=0;i<size;i++)
{
	 z=0;
	for (int j=0;j<row_kernelweight*size;j++)
	{
		if (j<size)
		{
			Final[i][j]+=outmatr.read();
			z++;
		}
		else if(j<2*size)
		{
			Final[i][j-z]+=outmatr.read();
		}
		else if (j<3*size)
		{
			Final[i][j-z-size]+=outmatr.read();
		}
		else if (j<4*size)
		{
			Final[i][j-z-(size*2)]+=outmatr.read();

		}
		else if (j<5*size)
		{
				Final[i][j-z-(size*3)]+=outmatr.read();

		}
		else if (j<6*size)
			{
				Final[i][j-z-(size*4)]+=outmatr.read();

			}
		else if (j<7*size)
			{
				Final[i][j-z-(size*5)]+=outmatr.read();

			}
		else if (j<8*size)
			{
				Final[i][j-z-(size*6)]+=outmatr.read();

			}
		else if (j<9*size)
			{
				Final[i][j-z-(size*7)]+=outmatr.read();

			}
		else if (j<10*size)
			{
				Final[i][j-z-(size*8)]+=outmatr.read();

			}
		else if (j<11*size)
			{
				Final[i][j-z-(size*9)]+=outmatr.read();

			}
		else if (j<12*size)
			{
				Final[i][j-z-(size*10)]+=outmatr.read();

			}
		else if (j<13*size)
			{
				Final[i][j-z-(size*11)]+=outmatr.read();

			}
		else if (j<14*size)
			{
				Final[i][j-z-(size*12)]+=outmatr.read();

			}
		else if (j<15*size)
			{
				Final[i][j-z-(size*13)]+=outmatr.read();

			}
		else if (j<16*size)
			{
				Final[i][j-z-(size*14)]+=outmatr.read();

			}
		else if (j<17*size)
			{
				Final[i][j-z-(size*15)]+=outmatr.read();

			}
		else if (j<18*size)
			{
				Final[i][j-z-(size*16)]+=outmatr.read();

			}
		else if (j<19*size)
			{
				Final[i][j-z-(size*17)]+=outmatr.read();

			}
		else if (j<20*size)
			{
				Final[i][j-z-(size*18)]+=outmatr.read();

			}


		else if (j<21*size)
			{
				Final[i][j-z-(size*19)]+=outmatr.read();

			}
		else if (j<22*size)
			{
				Final[i][j-z-(size*20)]+=outmatr.read();

			}
		else if (j<23*size)
			{
				Final[i][j-z-(size*21)]+=outmatr.read();

			}
		else if (j<24*size)
			{
				Final[i][j-z-(size*22)]+=outmatr.read();

			}
		else if (j<25*size)
			{
				Final[i][j-z-(size*23)]+=outmatr.read();

			}
		else if (j<26*size)
			{
				Final[i][j-z-(size*24)]+=outmatr.read();

			}
		else if (j<27*size)
			{
				Final[i][j-z-(size*25)]+=outmatr.read();

			}
		else if (j<28*size)
			{
				Final[i][j-z-(size*26)]+=outmatr.read();

			}
		else if (j<29*size)
			{
				Final[i][j-z-(size*27)]+=outmatr.read();

			}
		else if (j<30*size)
			{
				Final[i][j-z-(size*28)]+=outmatr.read();

			}

		else if (j<31*size)
				{
					Final[i][j-z-(size*29)]+=outmatr.read();

				}
		else if (j<32*size)
				{
					Final[i][j-z-(size*30)]+=outmatr.read();

				}
		else if (j<33*size)
				{
					Final[i][j-z-(size*31)]+=outmatr.read();

				}
		else if (j<34*size)
				{
					Final[i][j-z-(size*32)]+=outmatr.read();

				}
		else if (j<35*size)
				{
					Final[i][j-z-(size*33)]+=outmatr.read();

				}
		else if (j<36*size)
				{
					Final[i][j-z-(size*34)]+=outmatr.read();

				}
		else if (j<37*size)
			   {
						Final[i][j-z-(size*35)]+=outmatr.read();

			   }

		else if (j<38*size)
					{
						Final[i][j-z-(size*36)]+=outmatr.read();

					}
		else if (j <39*size)
					{
						Final[i][j-z-(size*37)]+=outmatr.read();

					}
		else if (j <40*size)
					{
						Final[i][j-z-(size*38)]+=outmatr.read();

					}


		else if (j<41*size)
		{

			Final[i][j-z-(size*39)]+=outmatr.read();
		}


		else if (j<42*size)
			{

				Final[i][j-z-(size*40)]+=outmatr.read();
			}
		else if (j<43*size)
			{

				Final[i][j-z-(size*41)]+=outmatr.read();

			}
		else if (j<44*size)
					{

						Final[i][j-z-(size*42)]+=outmatr.read();

					}
		else if (j<45*size)
					{

						Final[i][j-z-(size*43)]+=outmatr.read();

					}
		else if (j<46*size)
					{

						Final[i][j-z-(size*44)]+=outmatr.read();

					}
		else if (j<47*size)
					{

						Final[i][j-z-(size*45)]+=outmatr.read();

					}
		else if (j<48*size)
					{

						Final[i][j-z-(size*46)]+=outmatr.read();

					}
		else if (j<49*size)
					{

						Final[i][j-z-(size*47)]+=outmatr.read();

					}
		else if (j<50*size)
					{

						Final[i][j-z-(size*48)]+=outmatr.read();

					}


		else if (j<51*size)
					{

						Final[i][j-z-(size*49)]+=outmatr.read();

					}
		else if (j<52*size)
					{

						Final[i][j-z-(size*50)]+=outmatr.read();

					}
		else if (j<53*size)
					{

						Final[i][j-z-(size*51)]+=outmatr.read();

					}
		else if (j<54*size)
					{

						Final[i][j-z-(52*size)]+=outmatr.read();

					}
		else if (j<55*size)
					{

						Final[i][j-z-(53*size)]+=outmatr.read();

					}
		else if (j<56*size)
					{

						Final[i][j-z-(size*54)]+=outmatr.read();

					}
		else if (j<57*size)
					{

						Final[i][j-z-(size*55)]+=outmatr.read();

					}
		else if (j<58*size)
					{

						Final[i][j-z-(size*56)]+=outmatr.read();

					}
		else if (j<59*size)
					{

						Final[i][j-z-(size*57)]+=outmatr.read();

					}
		else if (j<60*size)
					{

						Final[i][j-z-(size*58)]+=outmatr.read();

					}
		else if (j<61*size)
					{

						Final[i][j-z-(size*59)]+=outmatr.read();

					}
		else if (j<62*size)
					{

						Final[i][j-z-(size*60)]+=outmatr.read();

					}
		else if (j<63*size)
					{

						Final[i][j-z-(size*61)]+=outmatr.read();

					}
		else if (j<64*size)
					{

						Final[i][j-z-(size*62)]+=outmatr.read();

					}
		else if (j<65*size)
					{

						Final[i][j-z-(size*63)]+=outmatr.read();

					}
		else if (j<66*size)
					{

						Final[i][j-z-(size*64)]+=outmatr.read();

					}
		else if (j<67*size)
					{

						Final[i][j-z-(size*65)]+=outmatr.read();

					}
		else if (j<68*size)
					{

						Final[i][j-z-(size*66)]+=outmatr.read();

					}
		else if (j<69*size)
					{

						Final[i][j-z-(size*67)]+=outmatr.read();

					}
		else if (j<70*size)
						{

							Final[i][j-z-(68*size)]+=outmatr.read();

						}
		else if (j<71*size)
						{

							Final[i][j-z-(size*69)]+=outmatr.read();

						}
		else if (j<72*size)
						{

							Final[i][j-z-(size*70)]+=outmatr.read();

						}
		else if (j<73*size)
						{

							Final[i][j-z-(size*71)]+=outmatr.read();

						}
		else if (j<74*size)
						{

							Final[i][j-z-(size*72)]+=outmatr.read();

						}
		else if (j<75*size)
						{

							Final[i][j-z-(size*73)]+=outmatr.read();

						}
		else if (j<76*size)
						{

							Final[i][j-z-(size*74)]+=outmatr.read();

						}
		else if (j<77*size)
						{

							Final[i][j-z-(size*75)]+=outmatr.read();

						}
		else if (j<78*size)
						{

							Final[i][j-z-(size*76)]+=outmatr.read();

						}
		else if (j<79*size)
						{

							Final[i][j-z-(size*77)]+=outmatr.read();

						}
		else
						{

							Final[i][j-z-(size*78)]+=outmatr.read();
						}

		}}
#endif

for (int i=0; i<size;i++)
{
	for (int j=0; j<size;j++)
	{
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


std::cout<<"Test Passed"<<"\n";
std::cout<<"No of non zero elements="<<pp<<"\n";

return 0;
}
