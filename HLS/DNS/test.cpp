#include<iostream>
#include <stdio.h>
#include<ap_int.h>
#include<hls_stream.h>
using namespace std;
#include"header.h"
#include<fstream>
#include<chrono>
int A[size][size],B[size][size],C[size][size];
int Final[size][size];
double cpu_time_used;


void DNS(hls::stream<ap_uint<streamlength>>&a,hls::stream<ap_uint<streamlength>>&b,
		hls::stream<ap_uint<streamlength>>&c);
int main()
{
hls::stream<ap_uint<streamlength>> a("A");
hls::stream<ap_uint<streamlength>> b ("B");
hls::stream<ap_uint<streamlength>> c ("C ");
int i,j;

for (i = 0; i<size; i++){
      for (j=0; j<size;j++){
         if(i==j){
           A[i][j]=1;
           Final[i][j]=0;
         	 }
         else{
           A[i][j]=0;
           Final[i][j]=0;
         }
        }
      }
      for (i = 0; i<size; i++){
        for (j=0; j<size;j++){
         if(i==j){
           B[i][j]=1;
           C[i][j]=0;
         }
         else{
        	 C[i][j]=0;
           B[i][j]=0;
         }
        }
      }

int pp=0;
int qp=0;
      for (int i=0;i<size;i++)
          {
    	  for (int k=0 ;k<size;k++)
    	     {
          for (int j=0;j<size;j++)
          {
        		a.write(A[i][j]);
        		pp++;

          } 	}
     }

      for (int k=0;k<size;k++)
      {

      for (int j=0;j<size;j++)
      {
    	  for (int i=0;i<size;i++)
    	  {

    		  b.write(A[i][j]);
    		  qp++;
    	  }
      }
     }


auto start = std::chrono::high_resolution_clock::now();

  for (int i=0;i<size;i++)
      {
      for (int j=0;j<size;j++)
      {

    	for (int k=0;k<size;k++)
    	{

    		C[i][j]+=A[i][k]*B[k][j];
    	}
      }
      }
auto finish = std::chrono::high_resolution_clock::now();

std::chrono::duration<long double> elapsed = finish - start;
std::cout << "Elapsed time: " << elapsed.count() << " s\n";
std::cout<<"Row kernel stream weight ="<<pp<<"\n";
std::cout<<"Col kernel stream weight ="<<qp<<"\n";

  DNS(a,b,c);



  for (int i=0;i<size;i++)
      {
      for (int j=0;j<size;j++)
      {

    	for (int k=0;k<size;k++)
    	{

    		Final[i][j]+=c.read();
    	}
      }
      }

ofstream Cmatrix ("Normalcomputation.txt");
     for (int k=0 ;k<size;k++)
     {
    	 for (int q=0; q<size;q++)
    	 {
    		Cmatrix<<"C"<<"["<<k<<"]"<<"["<<q<<"]"<<"="<<C[k][q]<<"\n";

    	 }
     }
Cmatrix.close();
ofstream Fmatrix ("Afterstream.txt");


     for (int k=0 ;k<size;k++)
     {
    	 for (int q=0; q<size;q++)
    	 {
    		Fmatrix<<"C"<<"["<<k<<"]"<<"["<<q<<"]"<<"="<<C[k][q]<<"\n";
    	 }
     }
Fmatrix.close();
     for (int k=0 ;k<size;k++)
     {
    	 for (int q=0; q<size;q++)
    	 {
    		if(C[k][q]!=Final[k][q])
    		{
    			std::cout<<"Fail";
    			return 0;
    		}
    	 }
     }

     std::cout<<"Pass"<<"\n";






    return 0;
}
