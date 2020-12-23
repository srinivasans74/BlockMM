void mmultsparse(const int *in1,const int *in2,int *out_r) {

  int B[row_weight][MAX_SIZE];
  int A[MAX_SIZE][row_weight];
  int C[MAX_SIZE][MAX_SIZE]={0};
  int tempsum[size];
  
  /*
  For smaller matrices use the below directive
*/
//#pragma HLS ARRAY_PARTITION variable = B dim = 2 complete
//#pragma HLS ARRAY_PARTITION variable = C dim = 2 complete
//#pragma HLS ARRAY_PARTITION variable = tempsum dim = 1 complete

#pragma HLS ARRAY_PARTITION variable=B dim=2 factor=8 cyclic
#pragma HLS ARRAY_PARTITION variable=C dim=2 factor=8 cyclic
#pragma HLS ARRAY_PARTITION variable=tempsum dim=1 factor=8 cyclic

/*
Data transfer
*/
  readvalA: for (int itr = 0, i = 0, j = 0; itr < size * row_weight; itr++, j++) {
    if (j == row_weight) {
      j = 0;
      i++;
    }
    A[i][j] = in1[itr];
  }


  readb:   for (int itr = 0, i = 0, j = 0; itr < size * row_weight; itr++, j++) {
      if (j == MAX_SIZE) {
        j = 0;
        i++;
      }
      B[i][j]= in2[itr];
    }


/*

Compute
*/

  lreorder1:    for (int i = 0; i < size; i++) {

      lreorder2:
        for (int k = 0; k < row_weight; k++) {
#pragma HLS PIPELINE
        lreorder3:
		for (int j = 0; j < MAX_SIZE; j++) {

            int result = (k == 0) ? 0 : tempsum[j];
            result += A[i][k] * B[k][j];
            tempsum[j] = result;
            if (k == row_weight - 1)
              C[i][j] = result;
          }
        }
      }


/*
Data transfer
*/

writeC:
  for (int itr = 0, i = 0, j = 0; itr < size * size; itr++, j++) {
    if (j == size) {
      j = 0;
      i++;
    }
    out_r[itr] = C[i][j];
  }
}

