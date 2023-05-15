/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_customFilter_api.c
 *
 * Code generation for function 'customFilter'
 *
 */

/* Include files */
#include "_coder_customFilter_api.h"
#include "_coder_customFilter_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;

emlrtContext emlrtContextGlobal = {
    true,                                                 /* bFirstTime */
    false,                                                /* bInitialized */
    131642U,                                              /* fVersionInfo */
    NULL,                                                 /* fErrorFunction */
    "customFilter",                                       /* fFunctionName */
    NULL,                                                 /* fRTCallStack */
    false,                                                /* bDebugMode */
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, /* fSigWrd */
    NULL                                                  /* fSigMem */
};

/* Function Declarations */
static int32_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                    const emlrtMsgIdentifier *parentId))[32];

static int32_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[32];

static int32_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *in,
                                  const char_T *identifier))[32];

static const mxArray *emlrt_marshallOut(const real_T u[32]);

/* Function Definitions */
static int32_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                    const emlrtMsgIdentifier *parentId))[32]
{
  int32_T(*y)[32];
  y = c_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

static int32_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[32]
{
  static const int32_T dims = 32;
  int32_T(*ret)[32];
  int32_T i;
  boolean_T b = false;
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "int32", false, 1U,
                            (const void *)&dims, &b, &i);
  ret = (int32_T(*)[32])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

static int32_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *in,
                                  const char_T *identifier))[32]
{
  emlrtMsgIdentifier thisId;
  int32_T(*y)[32];
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(in), &thisId);
  emlrtDestroyArray(&in);
  return y;
}

static const mxArray *emlrt_marshallOut(const real_T u[32])
{
  static const int32_T i = 0;
  static const int32_T i1 = 32;
  const mxArray *m;
  const mxArray *y;
  y = NULL;
  m = emlrtCreateNumericArray(1, (const void *)&i, mxDOUBLE_CLASS, mxREAL);
  emlrtMxSetData((mxArray *)m, (void *)&u[0]);
  emlrtSetDimensions((mxArray *)m, &i1, 1);
  emlrtAssign(&y, m);
  return y;
}

void customFilter_api(const mxArray *prhs, const mxArray **plhs)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  real_T(*out)[32];
  int32_T(*in)[32];
  st.tls = emlrtRootTLSGlobal;
  out = (real_T(*)[32])mxMalloc(sizeof(real_T[32]));
  /* Marshall function inputs */
  in = emlrt_marshallIn(&st, emlrtAlias(prhs), "in");
  /* Invoke the target function */
  customFilter(*in, *out);
  /* Marshall function outputs */
  *plhs = emlrt_marshallOut(*out);
}

void customFilter_atexit(void)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  customFilter_xil_terminate();
  customFilter_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

void customFilter_initialize(void)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, NULL);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

void customFilter_terminate(void)
{
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/* End of code generation (_coder_customFilter_api.c) */
