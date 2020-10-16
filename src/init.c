#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* FIXME:
 Check these declarations against the C/Fortran source code.
 */

/* .Call calls */
extern SEXP export_new_hashed_environment(SEXP, SEXP);
extern SEXP export_test_many_envs(SEXP, SEXP, SEXP, SEXP);

static const R_CallMethodDef CallEntries[] = {
  {"export_new_hashed_environment", (DL_FUNC) &export_new_hashed_environment, 2},
  {"export_test_many_envs",         (DL_FUNC) &export_test_many_envs, 4},
  {NULL, NULL, 0}
};

void R_init_testenvsxp(DllInfo *dll)
{
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
