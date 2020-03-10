#include "Header.h"


/* this function generates random numbers with 
Gaussian distribution based on Box-Muller transform
and fills in the input array with them
*/
void Gaussian_random_generator(double* vec, unsigned int size, double mu, double sigma)
{
	double eps = numeric_limits<double>::min();
	double two_pi = 2.0 * 3.14159265358979323846;
	double n1, n2, u1, u2;

	for (unsigned int i = 0; i < size; i += 2)
	{
		do
		{
			u1 = rand() * (1.0 / RAND_MAX);
			u2 = rand() * (1.0 / RAND_MAX);
		}
		while (u1 <= eps);

		n1 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
		n2 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);

		vec[i] = n1; 
		if (i + 1 < size) vec[i + 1] = n2;
	}
}


/* finding the minimum and maximum of the input array */
void limit_finder(double* vec, unsigned int size, double& min, double& max)
{
	min = vec[0];
	max = vec[0];

	for (int i = 1; i < size; i++)
	{
		if (vec[i] < min) min = vec[i];
		if (vec[i] > max) max = vec[i];
	}
}



/* this function finds the correct bin for each data in the
input array. The distance between min and max of the array
is divided into num_bin equally sized bins. */

int bin_finder(double* val, double len_bin, double min, double max)
{
	for (int i = 0; i < num_bin; i++)
	{
		if ((val[0] >= min + i * len_bin) && (val[0] < min + (i + 1) * len_bin))
		{
			return i;
		}
		else if (max == val[0])
		{
			return num_bin - 1;
		}
	}
}



/* sequential histogram */

void seq1(double* vec, unsigned int len_vec, double min, double max, unsigned int* histogram)
{
	double len_bin = (double)(max - min) / num_bin;
	for (int i = 0; i < num_bin; i++)
	{
		histogram[i] = 0;
	}

	for (int i = 0; i < len_vec; i++)
	{
		histogram[bin_finder(&vec[i], len_bin, min, max)]++;
	}
}



/* first parallel histogram. Each bin is updated using atomic add */

void par1(double* vec, unsigned int len_vec, double min, double max, unsigned int* histogram)
{
	omp_set_num_threads(NUM_THREAD);

	double len_bin = (double)(max - min) / num_bin;

#pragma omp parallel for
	for (int i = 0; i < num_bin; i++)
	{
		histogram[i] = 0;
	}

#pragma omp parallel for
	for (int i = 0; i < len_vec; i++)
	{
#pragma omp atomic
		histogram[bin_finder(&vec[i], len_bin, min, max)]++;
	}
}



/* second implementation of parallel histogram. Every time a bin
is going to be updated, a lock is set on that bin. After the bin
is updated, the lock is released.*/

void par2(double* vec, unsigned int len_vec, double min, double max, unsigned int* histogram)
{
	omp_set_num_threads(NUM_THREAD);

	double len_bin = (double)(max - min) / num_bin;
	omp_lock_t hist_lock[num_bin];
	int i_bin;

#pragma omp parallel for
	for (int i = 0; i < num_bin; i++)
	{
		histogram[i] = 0; omp_init_lock(&hist_lock[i]);  // initialize locks
	}

#pragma omp parallel for private(i_bin)
	for (int i = 0; i < len_vec; i++)
	{
		i_bin = bin_finder(&vec[i], len_bin, min, max);
		omp_set_lock(&hist_lock[i_bin]);  // set locks
		histogram[i_bin]++;
		omp_unset_lock(&hist_lock[i_bin]); // release locks
	}

	for (int i = 0; i < num_bin; i++)
	{
		omp_destroy_lock(&hist_lock[i]); // destroy locks
	}
}



/* In this third version of the parallel histogram, each thread
has its own private histogram. Therefore, there is no need for 
locks or synchronization. All private histograms are finally
combined to a single histogram using the atomic add */

void par3(double* vec, unsigned int len_vec, double min, double max, unsigned int* histogram)
{
	omp_set_num_threads(NUM_THREAD);

	double len_bin = (double)(max - min) / num_bin;

#pragma omp parallel for
	for (int i = 0; i < num_bin; i++)
	{
		histogram[i] = 0;
	}

#pragma omp parallel
	{
		int priv_hist[num_bin];
		for (int i = 0; i < num_bin; i++)
		{
			priv_hist[i] = 0;
		}

#pragma omp for nowait
		for (int i = 0; i < len_vec; i++)
		{
			priv_hist[bin_finder(&vec[i], len_bin, min, max)]++;
		}

		for (int j = 0; j < num_bin; j++)
		{
#pragma omp atomic
			histogram[j] += priv_hist[j];
		}
	}
}


/* Checking the accuracy of parallel histograms based on
the result of the sequential histogram */

bool check(unsigned int* vec1, unsigned int* vec2)
{
	bool flag = true;
	for (int i = 0; i < num_bin; i++)
	{
		if (vec1[i] != vec2[i])
		{
			flag = false;
		}
	}

	return flag;
}



/* a function for plotting the histogram */

void plot(unsigned int* histogram)
{
	unsigned int scale_hor = 120; // horizontal scaling
	unsigned int size_vec = num_bin > scale_hor ? scale_hor : num_bin;
	vector<unsigned int> vec_hist(size_vec, 0); // scaled histogram vector
	double step = (double) size_vec/num_bin;

	if (num_bin == size_vec)
	{
		for (int i = 0; i < num_bin; i++)
		{
			vec_hist[i] = histogram[i];
		}
	}
	else
	{
		int j = 0;
		for (int i = 0; i < num_bin; i++)
		{
			if (i * step >= j && i * step < j + 1)
			{
				vec_hist[j] += histogram[i];
			}
			else
			{
				j++; vec_hist[j] += histogram[i];
			}
		}
	}

	if (LOG)
	{
		for (auto& i : vec_hist) { cout << i << "  "; }
		cout << endl;
	}
	
	unsigned int scale_ver = 20;  // vertical scaling
	unsigned int max_bin = 0;
	for (auto& i: vec_hist) { if (i > max_bin) max_bin = i; }
	if (LOG)cout << "max_bin= " << max_bin << endl;
	unsigned int height_vec = scale_ver > max_bin ? max_bin: scale_ver;
	if (LOG)cout << "height_vec= " << height_vec << endl;
	for (int i = 0; i < size_vec; i++)
	{
		vec_hist[i] = vec_hist[i] * height_vec / max_bin;
	}

	if (LOG)
	{
		for (auto& i : vec_hist) { cout << i << "   "; }
		cout << endl;
	}

	cout << endl;  // plotting
	for (unsigned int i = height_vec; i > 0; i--)
	{
		for (auto& j : vec_hist)
		{
			if (j >= i) { cout << "@"; }
			else { cout << " "; }
		}
		cout << endl;
	}
}