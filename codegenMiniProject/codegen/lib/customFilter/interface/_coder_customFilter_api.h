/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_customFilter_api.h
 *
 * Code generation for function 'customFilter'
 *
 */

#ifndef _CODER_CUSTOMFILTER_API_H
#define _CODER_CUSTOMFILTER_API_H

/* Include files */
#include "emlrt.h"
#include "tmwtypes.h"
#include <string.h>

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void customFilter(int32_T in[32], real_T out[32]);

void customFilter_api(const mxArray *prhs, const mxArray **plhs);

void customFilter_atexit(void);

void customFilter_initialize(void);

void customFilter_terminate(void);

void customFilter_xil_shutdown(void);

void customFilter_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (_coder_customFilter_api.h) */
