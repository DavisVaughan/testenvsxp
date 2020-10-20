#' @export
new_hashed_environment <- function(size = 29L, parent = parent.frame()) {
  .Call(export_new_hashed_environment, size, parent)
}

#' @export
test_many_envs <- function(times, method, size = 29L, parent = parent.frame()) {
  .Call(export_test_many_envs, times, method, size, parent)
}
