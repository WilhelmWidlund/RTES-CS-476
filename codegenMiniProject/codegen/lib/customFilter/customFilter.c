/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * customFilter.c
 *
 * Code generation for function 'customFilter'
 *
 */

/* Include files */
#include "customFilter.h"
#include <string.h>

/* Function Definitions */
void customFilter(const int in[32], double out[32])
{
  static const double dv[31] = {
      -0.00049573627107127793, 0.0020413077191851897,   0.004991197258361238,
      -4.8926230700263321E-18, -0.011435962459741164,   -0.0098379439690512351,
      0.004108953279864774,    -4.7795307556093163E-18, -0.0078089496336161985,
      0.0360802796038416,      0.0834303488008098,      -2.4712018402001212E-17,
      -0.1647203691874897,     -0.15236609508070154,    0.092016202253481533,
      0.24924848789999515,     0.092016202253481533,    -0.15236609508070154,
      -0.1647203691874897,     -2.4712018402001212E-17, 0.0834303488008098,
      0.0360802796038416,      -0.0078089496336161985,  -4.7795307556093163E-18,
      0.004108953279864774,    -0.0098379439690512351,  -0.011435962459741164,
      -4.8926230700263321E-18, 0.004991197258361238,    0.0020413077191851897,
      -0.00049573627107127793};
  int j;
  int k;
  int naxpy;
  memset(&out[0], 0, 32U * sizeof(double));
  for (j = 0; j < 31; j++) {
    out[j] += (double)in[0] * dv[j];
  }
  naxpy = 30;
  for (k = 0; k < 31; k++) {
    for (j = 0; j <= naxpy; j++) {
      int out_tmp;
      out_tmp = (k + j) + 1;
      out[out_tmp] += (double)in[k + 1] * dv[j];
    }
    naxpy--;
  }
}

/* End of code generation (customFilter.c) */
