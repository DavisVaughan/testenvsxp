
<!-- README.md is generated from README.Rmd. Please edit that file -->

# testenvsxp

``` r
library(testenvsxp)

custom <- new_hashed_environment()
base_hashed <- new.env(hash = TRUE)
base_not_hashed <- new.env(hash = FALSE)

pairs <- setNames(as.list(1:1000), paste0("A", 1:1000))

rlang::env_bind(custom, !!!pairs)
rlang::env_bind(base_hashed, !!!pairs)
rlang::env_bind(base_not_hashed, !!!pairs)

# proof that we have a hashed env
bench::mark(
  custom$A1,
  base_hashed$A1,
  base_not_hashed$A1,
  iterations = 100000
)
#> # A tibble: 3 x 6
#>   expression              min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr>         <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 custom$A1             163ns    204ns  4157550.        0B    125. 
#> 2 base_hashed$A1        160ns    199ns  4371682.        0B    175. 
#> 3 base_not_hashed$A1   2.44µs   2.63µs   347727.        0B     10.4

# proof that `child` has `custom` as a parent env
custom$x <- 1
child <- new_hashed_environment(parent = custom)
eval(quote(x), child)
#> [1] 1

# generate `times` environments at the C level
bench::mark(
  base = test_many_envs(times = 1e5L, base = TRUE, size = 29L), 
  custom = test_many_envs(times = 1e5L, base = FALSE, size = 29L), 
  iterations = 200
)
#> Warning: Some expressions had a GC in every iteration; so filtering is disabled.
#> # A tibble: 2 x 6
#>   expression      min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 base         76.3ms     92ms      10.5    26.7MB     14.8
#> 2 custom       16.6ms   21.5ms      29.8    26.7MB     14.6

# with a very small size (when we just need a small child env)
bench::mark(
  base = test_many_envs(times = 1e5L, base = TRUE, size = 3L), 
  custom = test_many_envs(times = 1e5L, base = FALSE, size = 3L), 
  iterations = 200
)
#> Warning: Some expressions had a GC in every iteration; so filtering is disabled.
#> # A tibble: 2 x 6
#>   expression      min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 base         50.7ms  63.69ms      15.6        0B     18.2
#> 2 custom        3.7ms   4.13ms     223.         0B     32.4
```
