new_hashed_environment <- function(size = 29L, parent = parent.frame()) {
  .Call(export_new_hashed_environment, size, parent)
}

test_many_envs <- function(times, size = 29L, parent = parent.frame(), base = FALSE) {
  .Call(export_test_many_envs, times, size, parent, base)
}
