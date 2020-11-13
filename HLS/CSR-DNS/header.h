/*
 *  * Fix the Matrix Size
 * No of nonzero elements for every row
*/



#define size 730
#define row_kernelweight 80
#define matrdim size*size*row_kernelweight

#if size==4
#define streamlength 16
#define dim 16
#elif size==8
#define streamlength 16
#define dim 32
#elif size==16
#define streamlength 16
#define dim 32
#elif size==32
#define dim 64
#define streamlength 16
#define partfactor 8
#elif size== 64
#define dim 256
#define streamlength 16
#elif size==128
#define dim 384
#define streamlength 16
#elif size==256
#define dim 512
#define streamlength 16
#elif size==512
#define dim 1024
#define streamlength 16
#elif size==560
#define dim 44800
#define streamlength 5
#elif size==720
#define dim 2880
#define streamlength 16
#elif size==730
#define dim 58400
#define streamlength 5

#endif
