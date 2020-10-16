#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------

#define SET_HASHPRI SET_TRUELENGTH
#define HASHMINSIZE 29

// - Assume size >0 (we could check for this)
// - Creates the hash table, which is a VECSXP with its
//   truelength set to 0
static inline SEXP new_hash_table(int size) {
  if (size <= 0) {
    size = HASHMINSIZE;
  }

  SEXP out = PROTECT(Rf_allocVector(VECSXP, size));
  SET_HASHPRI(out, 0);

  UNPROTECT(1);
  return out;
}

#undef SET_HASHPRI
#undef HASHMINSIZE


// -----------------------------------------------------------------------------

static inline SEXP new_hashed_environment(int size, SEXP parent) {
  // Allocate non-hashed env
  SEXP out = PROTECT(Rf_allocSExp(ENVSXP));

  // Make it hashed by setting the hash table
  SET_HASHTAB(out, new_hash_table(size));

  // Set parent env
  SET_ENCLOS(out, parent);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

// No checks
SEXP export_new_hashed_environment(SEXP size, SEXP parent) {
  int c_size = INTEGER(size)[0];
  return new_hashed_environment(c_size, parent);
}

// -----------------------------------------------------------------------------

SEXP export_test_many_envs(SEXP times, SEXP size, SEXP parent, SEXP base) {
  int c_times = INTEGER(times)[0];
  int c_size = INTEGER(size)[0];
  bool c_base = LOGICAL(base)[0];

  //SEXP out = PROTECT(Rf_allocVector(VECSXP, c_times));
  SEXP out = R_NilValue;

  if (c_base) {
    SEXP call = PROTECT(
      Rf_lang4(Rf_install("new.env"), Rf_ScalarLogical(true), parent, size)
    );

    for (int i = 0; i < c_times; ++i) {
      //SET_VECTOR_ELT(out, i, Rf_eval(call, R_BaseEnv));
      Rf_eval(call, R_BaseEnv);
    }

    //UNPROTECT(2);
    UNPROTECT(1);
    return out;
  }

  for (int i = 0; i < c_times; ++i) {
    //SET_VECTOR_ELT(out, i, new_hashed_environment(c_size, parent));
    new_hashed_environment(c_size, parent);
  }

  //UNPROTECT(1);
  return out;
}
