#pragma once

#include<iostream>
#include<omp.h>
#include<vector>

#define LOG false

using namespace std;

void Gaussian_random_generator(double* vec, unsigned int size, double mu, double sigma);
void limit_finder(double* vec, unsigned int size, double& min, double& max);
int bin_finder(double* val, double len_bin, double min, double max);
void seq1(double* vec, unsigned int len_vec, double min, double max, unsigned int* histogram);
void par1(double* vec, unsigned int len_vec, double min, double max, unsigned int* histogram);
void par2(double* vec, unsigned int len_vec, double min, double max, unsigned int* histogram);
void par3(double* vec, unsigned int len_vec, double min, double max, unsigned int* histogram);
bool check(unsigned int* vec1, unsigned int* vec2);
void plot(unsigned int* histogram);

const int NUM_THREAD = 12;
const int num_bin = 120;