#include "utils.hpp"
#include <cmath>
#include <iostream>

float vec_compute_distance(const float* u, const float* v, size_t dim){
    float d = 0;
    float s = 0;
    float diff = 0;
    for (size_t i = 0;  i < dim; i++){
        diff = (u[i] - v[i]);
        s = s + diff * diff;
    }
    d = std::sqrt(s);
    return d;
}

void vec_add(float* s, const float* u,  size_t dim){
    for (size_t i = 0;  i < dim; i++){
        s[i] = u[i] + s[i];
    }
}

void vec_divide(float* u, float scalar, size_t dim){
    for (size_t i = 0;  i < dim; i++){
        u[i] = u[i] / scalar;
    }
}

void set_array(float *array, size_t dim, float val){
    for (size_t i = 0;  i < dim; i++){
        array[i] = val;
    }
}

float vec_compute_avg_dif(const float *u, const float* v,  size_t dim){

    float dif = 0;
    for (size_t i = 0;  i < dim; i++){        
        dif = dif + std::abs(u[i] - v[i]);
        //std::cout << u[i] << " - " << v[i] << "  " << std::abs(u[i] - v[i]) << std::endl;
    }   
    //std::cout << dif <<std::endl;
    return dif / dim;
}

void prsize_t_array(const float *array, size_t d){
    for (size_t i = 0; i< d; i ++){
        std::cout << array[i] << " ";
    }
    std::cout<< std::endl;
}

//funciones auxiliares para la ordenacion, sacadas del codigo base mas modificacion propia

size_t getRandomsize_t(size_t min, size_t max){
	float a = rand() / static_cast<float>(RAND_MAX);
	return static_cast<size_t>(a * (max - min) + min + 0.5);
}


void size_t_swap(size_t* B, size_t i, size_t j){
	size_t aux = B[i];
	B[i] = B[j];
	B[j] = aux;
}

void float_swap(float* A, size_t i, size_t j){
	float aux = A[i];
	A[i] = A[j];
	A[j] = aux;
}

size_t split(float* A, size_t* B, size_t i, size_t j){
    size_t p = getRandomsize_t(i, j);
    
	while (i < j){
        while (i < p && A[i] <= A[p]){
            i = i + 1;
        }
        while (j > p && A[j] >= A[p]){
            j = j - 1;
        }
        float_swap(A, i, j);
        size_t_swap(B, i, j);

        if (i == p) {
            p = j;
        }
        else if (j == p){
            p = i;
        }    
    }  	
	return p;
}

//quickSort adjusted to sort the indexes array

void quickArgsort(float* A, size_t* B, size_t i, size_t j){
	if (i < j){
		size_t k = split(A, B, i, j);
        if (k > 0 && k > i) {
		quickArgsort(A, B, i, k-1);
        }
        if (k + 1 <= j) {
            quickArgsort(A, B, k + 1, j);
        }
	}
}

void quickArgsort(float* A, size_t* B, size_t n){
    if (n == 0) return;
	quickArgsort(A, B, 0, n - 1);
}

void partialArgsort(float* A, size_t* B, size_t i, size_t j, size_t k){
    if (i >= j) return;
	size_t p = split(A, B, i, j);
	if (k == p){
		return;
	}
	else if (k < p && p > 0 && p > i){
		partialArgsort(A, B, i, p-1, k);
	}
	else if (p + 1 <= j){
		partialArgsort(A, B, p+1, j, k);
	}
}

void partialArgsort(float* A, size_t* B, size_t n, size_t k){
    if (n == 0 || k >= n) return;
	partialArgsort(A, B, 0, n-1, k);
}

void mainSort(float* A, size_t* B, size_t i, size_t j, size_t k){
    partialArgsort(A, B, i, j, k);
    if (k > 0) {
        quickArgsort(A, B, i, i + k -1);
    }
}

void mainSort(float* A, size_t* B, size_t n, size_t k){
    if (n == 0 || k == 0) return;
    if (k > n) k = n;
    mainSort(A, B, 0, n-1, k);
}
