
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
#> 1 custom$A1             161ns    221ns  3856408.        0B    116. 
#> 2 base_hashed$A1        178ns    221ns  3933458.        0B    157. 
#> 3 base_not_hashed$A1   2.19µs   2.35µs   378758.        0B     11.4

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
#> 1 base         93.5ms  105.7ms      9.10    26.7MB     12.8
#> 2 custom       18.4ms   23.8ms     25.8     26.7MB     12.6

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
#> 1 base        49.71ms  68.25ms      14.5        0B     16.8
#> 2 custom       3.85ms   4.25ms     217.         0B     31.4
```
