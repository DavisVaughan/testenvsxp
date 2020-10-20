
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
#> 1 custom$A1             166ns    229ns  3945783.        0B   118.  
#> 2 base_hashed$A1        160ns    206ns  4235873.        0B   169.  
#> 3 base_not_hashed$A1   2.29µs   2.51µs   330334.        0B     9.91

# proof that `child` has `custom` as a parent env
custom$x <- 1
child <- new_hashed_environment(parent = custom)
eval(quote(x), child)
#> [1] 1

# generate `times` environments at the C level
bench::mark(
  callback = test_many_envs(times = 1e5L, method = "callback", size = 29L), 
  custom = test_many_envs(times = 1e5L, method = "custom", size = 29L), 
  internals = test_many_envs(times = 1e5L, method = "internals", size = 29L), 
  iterations = 200
)
#> Warning: Some expressions had a GC in every iteration; so filtering is disabled.
#> # A tibble: 3 x 6
#>   expression      min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 callback     79.3ms   97.1ms      10.1    26.7MB     16.8
#> 2 custom       14.3ms   18.9ms      32.0    26.7MB     10.5
#> 3 internals    14.2ms     19ms      31.9    26.7MB     10.4

# with a very small size (when we just need a small child env)
bench::mark(
  callback = test_many_envs(times = 1e5L, method = "callback", size = 3L), 
  custom = test_many_envs(times = 1e5L, method = "custom", size = 3L), 
  internals = test_many_envs(times = 1e5L, method = "internals", size = 3L), 
  iterations = 200
)
#> Warning: Some expressions had a GC in every iteration; so filtering is disabled.
#> # A tibble: 3 x 6
#>   expression      min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 callback    61.17ms  67.94ms      14.4        0B     16.6
#> 2 custom       3.89ms   4.26ms     220.         0B     30.8
#> 3 internals    3.42ms    3.9ms     224.         0B     31.4
```
