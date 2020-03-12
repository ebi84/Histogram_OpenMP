# Histogram_OpenMP
Parallel implementation of histogram using OpenMP

This C++/OpenMP code implements three versions of parallel histogram and compares them to sequential histogram. The array `vec` is
initialized with random values with normal distribution. This is the input vector and can obviously contain any distribution.
The function `limit_finder` finds the minimum and maximum values of `vec`. The distance between `min` and `max` found by `limit_finder`
function is then uniformly divided into `num_bin` portions. Function `bin_finder` is responsible for finding the proper bin for any
element of the input array.
