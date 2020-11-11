//Source Code for Matrix Multiplication in C without using function

#include <stdio.h>
int a[560][560];
int b[560][560];
int c[560][560],d[560][560],res[560][560]={0};
unsigned long read_cycles(void)
{
  unsigned long cycles;
  asm volatile  ("rdcycle %0" : "=r" (cycles));
  return cycles;
}

int main()
{
    int m, n,i,j,k, p, q,bi,bj,bk;

unsigned long start,end, total;
  //  int second[7][7]={2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
    //int first[7][7]={2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
      //  int multiply[7][7];
      for (i = 0; i<560; i++){
        for (j=0; j<560;j++){
         if(i==j){
           a[i][j]=1;
         }
         else{
           a[i][j]=0;
         }
        }
      }
      for (i = 0; i<560; i++){
        for (j=0; j<560;j++){
         if(i==j){
           b[i][j]=1;
         }
         else{
           b[i][j]=0;
         }
        }
      }
    m=q=560;
    n = 558;
start= read_cycles();
        //Carrying out matrix multiplication operation
        for(bi=0; bi<m; bi+=5)
        for(bj=0; bj<q; bj+=2)
            for(bk=0; bk<n; bk+=6)
                for(i=0; i<5; i++)
                    for(j=0; j<2; j++)
                        for(k=0; k<6; k++)
                            c[bi+i][bj+j] += a[bi+i][bk+k]*b[bk+k][bj+j];
for(bi=0; bi<560; bi+=5)
        for(bj=0; bj<560; bj+=5)
            for(bk=558; bk<560; bk+=2)
                for(i=0; i<5; i++)
                    for(j=0; j<5; j++)
                        for(k=0; k<2; k++)
                            d[bi+i][bj+j] += a[bi+i][bk+k]*b[bk+k][bj+j];
for (i = 0; i<560; i++){
        for (j=0; j<560;j++){
       res[i][j]=c[i][j]+d[i][j];
      }
}

end= read_cycles();

        //Printing the final product matrix

    total = (double)(end - start);
   printf("Total time taken on computation: %lu\n, %d,%d", total,res[55][55],res[507][505] );

    return 0;
}

