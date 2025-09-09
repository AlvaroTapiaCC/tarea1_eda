#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <cmath>

float vec_compute_distance(const float *u, const float* v, size_t dim);

void vec_add(float *s, const float* u,  size_t dim);

void vec_divide(float *u, float scalar, size_t dim);

void set_array(float *array, size_t dim, float val = 0);

float vec_compute_avg_dif(const float *u, const float* v,  size_t dim);

void prsize_t_array(const float *array, size_t d);

size_t getRandomsize_t(size_t min, size_t max);

void size_t_swap(size_t* A, size_t i, size_t j);

void float_swap(float* B, size_t i, size_t j);

size_t split(float* A, size_t* B, size_t i, size_t j);

void partialArgsort(float* A, size_t* B, size_t i, size_t j, size_t k);

void partialArgsort(float* A, size_t* B, size_t n, size_t k);

void quickArgsort(float* A, size_t* B, size_t i, size_t j);

void quickArgsort(float* A, size_t* B, size_t n);

void mainSort(float* A, size_t* B, size_t i, size_t j, size_t k);

void mainSort(float* A, size_t* B, size_t n, size_t k);

#endif