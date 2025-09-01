#include "matrix.hpp"
#include <string>
#include <fstream>
#include <cstdlib>

/** funtion to read a numpy array */
float* readNumpyData(const std::string &str_file, size_t* _n, size_t* _dim){
    float *data=nullptr;
    // open file and got to the end
    std::ifstream file(str_file, std::ios::binary |  std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << str_file << std::endl;
        return nullptr;
    }
    // read header information from the numpy data
    int pos_i = 51;
    int n_str = 20;
    char header[n_str];    
    for(int i =pos_i ; i<pos_i + n_str; i++){      
        header[i-pos_i] = 0 ;
    }    
    file.seekg(pos_i);    
    char c = ' ';
    for (int i = pos_i; i < pos_i +  n_str; i++){
        file.read(&c, 1);
        header[i-pos_i] = c;
    }
    // get n and dim
    std::string str_header(header);
    size_t pos_1 = str_header.find('(');
    size_t pos_2 = str_header.find(')');
    size_t pos_3 = str_header.find(',');
    std::string str_shape_0  = str_header.substr(pos_1 + 1, pos_3 - pos_1 - 1);
    std::string str_shape_1  = str_header.substr(pos_3 + 1, pos_2 - pos_3 - 1);
    int n = std::stoi(str_shape_0);
    int dim = std::stoi(str_shape_1);    
    std::cout<< "read data " << n << " x " << dim << std::endl;
    // read float data     
    file.seekg(128);    
    float value = 0;
    data = new float[n*dim];
    for (int i = 0 ;  i < n * dim ; i++){
        file.read(reinterpret_cast<char*>(&value), sizeof(float));
        data[i] = value;
    } 
    *_n = n;
    *_dim = dim;
    return data;
}

Matrix::Matrix(size_t _n, size_t _dim) : data(nullptr), n(_n), dim(_dim){
    if (n * dim > 0) {
        data = new float[n*dim];
        for(size_t i = 0 ; i < n * dim ; i++){
          data[i] = 0;
        }
    }
} 

Matrix::Matrix(const std::string &str_file) : data(nullptr), n(0), dim(0){
    data = readNumpyData(str_file, &n, &dim);
    std::cout << "data loaded ok " << std::endl; 
}

void Matrix::setAllRandom(){
    for(size_t i = 0 ; i < n * dim ; i++){
        data[i] = static_cast<float>(rand()) / RAND_MAX;
    }
}

const float* Matrix::getRow(size_t row) const{
    size_t pos = row * dim;
    return data + pos;
}

void Matrix::setRow(size_t row, float* vector){
    size_t pos = row * dim;
    for(size_t i = 0; i < dim; i++){
        data[pos + i] = vector[i];
    }
}
size_t Matrix::getN() const{
    return n;
}

size_t Matrix::getDim() const{
    return dim;
}

const float *Matrix::getData(){
    return data;
}
void Matrix::show() const {
    std::cout << "["; 
    size_t pos_row = 0;
    for(size_t i = 0; i < n  ; i ++){
        std::cout << "["; 
        pos_row = i * dim;
        for(size_t j = 0; j < dim - 1 ; j ++){ 
            std::cout << data[pos_row + j] <<", "; 
        }
        std::cout << data[pos_row + dim - 1] <<"],"; 
        std::cout << std::endl;
    }
    std::cout << "]" <<std::endl; 
    
}

Matrix::~Matrix(){    
    if (data != nullptr){        
        delete[] data;
        data = nullptr;        
    }    
}
