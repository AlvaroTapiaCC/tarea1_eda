#ifndef _MATRIX_HPP
#define _MATRIX_HPP
#include <iostream>
class Matrix{
    private:
        float *data; // buffer for storing the float data
        size_t n; // number of rows
        size_t dim; // dimension of the vectors
    public:
        Matrix(size_t _n = 0, size_t _dim  = 0);
        Matrix(const std::string &str_file);
        void setAllRandom();
        const float *getRow(size_t row) const;
        void setRow(size_t row, float* vector);
        void show() const;
        size_t getN() const;
        size_t getDim() const;
        const float *getData();
        virtual ~Matrix();
};

#endif