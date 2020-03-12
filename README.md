# Histogram_OpenMP
Parallel implementation of histogram using OpenMP

This C++/OpenMP code implements three versions of parallel histogram and compares them to sequential histogram. The array `vec` is
initialized with random values with normal distribution. This is the input vector and can obviously contain any distribution.
The function `limit_finder` finds the minimum and maximum values of `vec`. The distance between `min` and `max` found by `limit_finder`
function is then uniformly divided into `num_bin` portions. Function `bin_finder` is responsible for finding the proper bin for any
element of the input array. `bin_finder` is used by both sequential and parallel histograms.

## Parallel version #1: Atomic operation
This implementation uses OpenMP to speed up the process. To avoid the race condition, each bin is updated using an atomic operation.

## Parallel version #2: Using locks
Locks are used in this OpenMP implementation. Every time a bin is about to be incremented, a lock is set for that bin. After updating
that bin, the lock is released.

## Parallel version #3: Reduction
In this third version of the parallel histogram, each thread has its own private histogram. Therefore, there is no need for locks or synchronization. All private histograms are finally combined to a single histogram using the atomic add.
