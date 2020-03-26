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

# Comparison
The table below shows the speedup of the three parallel implementations of histogram for different vector sizes and different bin numbers with respect to the sequential implementation:

| bin/size |   1e3     |   1e5     |   1e7     |   1e8     | 
|----------|-----------|-----------|-----------|-----------|
|    16    |<p>version #1: -7.4%<br> version #2: -21.4%<br> version #3: 130.8%</p>|<p>version #1: 172.9%<br> version #2: -30.0%<br> version #3: 112.3%</p>|<p>version #1: 153.9%<br> version #2: -35.9%<br> version #3: 168.4%</p>|<p>version #1: 167.4%<br> version #2: -37.2%<br> version #3: 154.1%</p>| 
|    32    |<p>version #1: 19.3%<br> version #2: 35.8%<br> version #3: 91.9%</p>|<p>version #1: 254.1%<br> version #2: 56.6%<br> version #3: 225.5%</p>|<p>version #1: 251.8%<br> version #2: 50.6%<br> version #3: 262.3%</p>|<p>version #1: 287.4%<br> version #2: 37.0%<br> version #3: 269.5%</p>|  
|    64    |<p>version #1: 54.4%<br> version #2: 61.3%<br> version #3: 195.0%</p>|<p>version #1: 174.4%<br> version #2: 99.4%<br> version #3: 181.7%</p>|<p>version #1: 406.8%<br> version #2: 203.6%<br> version #3: 393.7%</p>|<p>version #1: 414.3%<br> version #2: 221.6%<br> version #3: 392.6%</p>|
