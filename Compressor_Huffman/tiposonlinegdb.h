#ifndef MEUSTIPOS
#define MEUSTIPOS
#include <stdbool.h>  // Para usar bool, true e false

// tipos inteiros
typedef char I8; // de -128 a 127
typedef unsigned char U8; // de 0 a 255
typedef short int I16; // de -32768 a 32767
typedef unsigned short int U16; // de 0 a 65535
typedef int I32; // de -2147483648 a 2147483647
typedef unsigned int U32; // de 0 a 4294967295
typedef long int I64; // de -9223372036854775808 a 9223372036854775807
typedef unsigned long int U64; // de 0 a 18446744073709551616

// tipos reais
typedef float R32; // de -3,4028235E+38 a 3.4028235E+38
typedef double R64; // de 2.2250738585072014E-308 a 1,7976931348623158E+308
typedef long double R128; // de 0.24651903288156618919116517665087070E-31 a 0.1797693134862315907729305190789002575e+309

#endif