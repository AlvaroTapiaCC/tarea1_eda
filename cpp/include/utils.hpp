#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <cmath>

float vec_compute_distance(const float *u, const float* v, size_t dim);

void vec_add(float *s, const float* u,  size_t dim);

void vec_divide(float *u, float scalar, size_t dim);

void set_array(float *array, size_t dim, float val = 0);

float vec_compute_avg_dif(const float *u, const float* v,  size_t dim);

void print_array(const float *array, size_t d);

int getRandomInt(int min, int max);

void swap(size_t* A, int i, int j);

int split(float* A, size_t* B, int i, int j);

void partialArgsort(float* A, size_t* B, int i, int j, int k, long long* c);

void partialArgsort(float* A, size_t* B, size_t n, int k, long long* c);

void quickArgsort(float* A, size_t* B, int i, int j, long long* c);

void quickArgsort(float* A, size_t* B, int n, long long* c);

#endif