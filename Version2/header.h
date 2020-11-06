#define size 4


#if size==4
#define streamlength 16
#define dim 8
#elif size==8
#define streamlength 16
#define dim 16
#elif size==16
#define streamlength 16
#define dim 32
#elif size==32
#define dim 64
#define streamlength 16
#define partfactor 8
#elif size== 64
#define dim 128
#define streamlength 16
#elif size==128
#define dim 256
#define streamlength 16
#elif size==256
#define dim 512
#define streamlength 16
#endif
