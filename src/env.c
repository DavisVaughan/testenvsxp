#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------

#define SET_HASHPRI SET_TRUELENGTH
#define HASHMINSIZE 29

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

// No checks on inputs
SEXP export_new_hashed_environment(SEXP size, SEXP parent) {
  int c_size = INTEGER(size)[0];
  return new_hashed_environment(c_size, parent);
}

// -----------------------------------------------------------------------------

// From Defn.h
SEXP R_NewHashedEnv(SEXP, SEXP);

// -----------------------------------------------------------------------------

SEXP export_test_many_envs(SEXP times, SEXP method, SEXP size, SEXP parent) {
  int c_times = INTEGER(times)[0];
  int c_size = INTEGER(size)[0];
  const char* c_method = CHAR(STRING_PTR(method)[0]);

  bool use_callback = !strcmp(c_method, "callback");
  bool use_internals = !strcmp(c_method, "internals");
  bool use_custom = !strcmp(c_method, "custom");

  if (use_callback) {
    // Callback to R
    SEXP call = PROTECT(
      Rf_lang4(Rf_install("new.env"), Rf_ScalarLogical(true), parent, size)
    );

    // Repeatedly call back to R to make a new hashed env
    for (int i = 0; i < c_times; ++i) {
      Rf_eval(call, R_BaseEnv);
    }

    UNPROTECT(1);
  } else if (use_internals) {
    for (int i = 0; i < c_times; ++i) {
      R_NewHashedEnv(parent, size);
    }
  } else if (use_custom) {
    for (int i = 0; i < c_times; ++i) {
      new_hashed_environment(c_size, parent);
    }
  } else {
    Rf_error("bad method");
  }

  return R_NilValue;
}
