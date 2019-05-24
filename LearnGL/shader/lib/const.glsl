#ifndef _CONST_
#define _CONST_

#define e          2.7182818
#define HALF_MAX        65504.0 // (2 - 2^-10) * 2^15
#define HALF_MAX_MINUS1 65472.0 // (2 - 2^-9) * 2^15
#define EPSILON         1.0e-5
#define PI              3.14159265359
#define TWO_PI          6.28318530718
#define FOUR_PI         12.56637061436
#define INV_PI          0.31830988618
#define INV_TWO_PI      0.15915494309
#define INV_FOUR_PI     0.07957747155
#define HALF_PI         1.57079632679
#define INV_HALF_PI     0.636619772367


#define FLT_EPSILON     1.192092896e-07 // Smallest positive number, such that 1.0 + FLT_EPSILON != 1.0
#define FLT_MIN         1.175494351e-38 // Minimum representable positive floating-point number
#define FLT_MAX         3.402823466e+38 // Maximum representable floating-point number



#ifdef COLORSPACE_GAMMA
#define ColorSpaceGrey fixed4(0.5, 0.5, 0.5, 0.5)
#define ColorSpaceDouble fixed4(2.0, 2.0, 2.0, 2.0)
#define ColorSpaceLuminance vec4(0.22, 0.707, 0.071, 0.0)
#else //line space
#define ColorSpaceGrey fixed4(0.214041144, 0.214041144, 0.214041144, 0.5)
#define ColorSpaceDouble fixed4(4.59479380, 4.59479380, 4.59479380, 2.0)
#define ColorSpaceLuminance vec4(0.0396819152, 0.458021790, 0.00609653955, 1.0)
#endif


#endif
