#include "Header.h"

int main()
{
	srand(0);
	int num_repeat = 10;
	unsigned int len_vec(10000000);
	double min, max;

	double* vec = new double[len_vec];
	unsigned int* histogram1 = new unsigned int[num_bin];
	unsigned int* histogram2 = new unsigned int[num_bin];
	unsigned int* histogram3 = new unsigned int[num_bin];
	unsigned int* histogram4 = new unsigned int[num_bin];

	double begin, end;
	double time_seq(0), time_par1(0), time_par2(0), time_par3(0);

	Gaussian_random_generator(vec, len_vec, 0, 1);
	limit_finder(vec, len_vec, min, max);

	begin = omp_get_wtime();
	for (int i = 0; i < num_repeat; i++)
	{
		seq1(vec, len_vec, min, max, histogram1);
	}
	end = omp_get_wtime();
	time_seq = (end - begin) * 1000 / num_repeat;
	cout << "calc time for sequential histogram (ms): " << time_seq << endl;

	begin = omp_get_wtime();
	for (int i = 0; i < num_repeat; i++)
	{
		par1(vec, len_vec, min, max, histogram2);
	}
	end = omp_get_wtime();
	time_par1 = (end - begin) * 1000 / num_repeat;
	cout << "calc time for the first parallel histogram (ms): " << time_par1 << endl;
	cout << "speedup compared to the sequential approach(%): " << (time_seq - time_par1) / time_par1 * 100 << endl;

	begin = omp_get_wtime();
	for (int i = 0; i < num_repeat; i++)
	{
		par2(vec, len_vec, min, max, histogram3);
	}
	end = omp_get_wtime();
	time_par2 = (end - begin) * 1000 / num_repeat;
	cout << "calc time for the second parallel histogram (ms): " << time_par2 << endl;
	cout << "speedup compared to the sequential approach(%): " << (time_seq - time_par2) / time_par2 * 100 << endl;

	begin = omp_get_wtime();
	for (int i = 0; i < num_repeat; i++)
	{
		par1(vec, len_vec, min, max, histogram4);
	}
	end = omp_get_wtime();
	time_par3 = (end - begin) * 1000 / num_repeat;
	cout << "calc time for the third parallel histogram (ms): " << time_par3 << endl;
	cout << "speedup compared to the sequential approach(%): " << (time_seq - time_par3) / time_par3 * 100 << endl;

	bool flag = check(histogram1, histogram2);
	if (flag) { cout << "Correct answer!" << endl; }
	flag = check(histogram1, histogram3);
	if (flag) { cout << "Correct answer!" << endl; }
	flag = check(histogram1, histogram4);
	if (flag) { cout << "Correct answer!" << endl; }

	plot(histogram1);

	delete[] vec;
	delete[] histogram1;
	delete[] histogram2;
	delete[] histogram3;
	delete[] histogram4;

	return 0;
}