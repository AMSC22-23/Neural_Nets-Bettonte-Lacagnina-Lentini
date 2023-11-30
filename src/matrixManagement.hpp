#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <typeinfo>


template<typename T>
void compareMatrix (size_t rows, size_t cols, T* result1, T* result2) {

	size_t size = rows*cols;
	size_t i = 0;

    for(i = 0; i < size; i++) {
        if(result1[i] != result2[i])
            break;
    }

    if(i == size) 
		std::cout << "Matrici uguali" << std::endl;
    else 
		std::cout << "Matrici diverse" << std::endl;
}


/**
 * Generates two uni-dimensional arrays of the specified type (float or double) and dimensions
 * @param rows, inners, cols for dimensions of arrays
 * @param type specifies type of elements
*/
template<typename T>
T* generateMatrix (size_t rows, size_t cols) {

	auto currentTime = std::chrono::high_resolution_clock::now();
	size_t dimension = rows * cols;
    unsigned int seed = static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count());
    
    std::mt19937 gen(seed); // Generator based on Mersenne Twister
    std::uniform_real_distribution<T> distribution(0.0, 100000.0); // uniform distribution between 0 and 100k
	
	T* matrix = new T[dimension];
	for (size_t i = 0; i < dimension; ++i) {
        matrix[i] = distribution(gen);
    }

    return matrix;
}

template<typename T>
void resetMatrix(T* matrix, int len){
	for(int i = 0; i < len; i++)
		matrix[i] = 0.0;
}

template<typename T>
void printMatrixCSV (const T* matrix, size_t rows, size_t cols) {

    size_t dimension = rows * cols;
    std::ofstream file("result.csv");

    if (file.is_open()) {
        for (size_t i = 0; i < dimension; i++) {

            if((i + 1) % cols == 0){
              file << matrix[i];
        		  file << std::endl;
            }
            else{
              file << matrix[i];
              file << ',';
            }
        }
        file.close();

    } else
        std::cerr << "File not opened" << std::endl;
}









