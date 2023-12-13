#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <typeinfo>

/**
 * This function compares two matrices in order to determine if they are equals or different.
 * 
 * @param result1  Pointer to the first matrix.
 * @param result1  Pointer to the first matrix.
 * @param rows     Number of rows of matrices.
 * @param columns  Number of columns of matrices.
 * 
 * @tparam T       Type of the matrix elements.
 */
template<typename T>
void compareMatrix (const size_t rows, const size_t cols, const T* result1, const T* result2) {
    const size_t size = rows*cols;
	size_t i;

    for(i = 0; i < size; i++)
        if(result1[i] != result2[i])
            break;
    
    if(i == size) 
		std::cout << "Matrici uguali" << std::endl;
    else 
		std::cout << "Matrici diverse" << std::endl;
}


/**
 * This function generates a uni-dimensional matrix of the specified type (float or double) and dimensions
 * 
 * @param rows    Number of rows of the matrix.
 * @param cols    Number of columns of the matrix.
 * @param type    Specifies type of elements.
 * 
 * @tparam T      Type of the matrix elements.
*/
template<typename T>
T* generateMatrix (const size_t rows, const size_t cols) {
	auto currentTime = std::chrono::high_resolution_clock::now();
	size_t dimension = rows * cols;
    unsigned int seed = static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count());
    
    std::mt19937 gen(seed); // Generator based on Mersenne Twister
    std::uniform_real_distribution<T> distribution(0.0, 1000.0); // uniform distribution between 0 and 100k
	
	T* matrix = new T[dimension];
	for (size_t i = 0; i < dimension; ++i) {
        matrix[i] = distribution(gen);
    }

    return matrix;
}


/**
 * This function sets elements of a matrix equal to zero.
 * 
 * @param matrix  Matrix to modify.
 * @param len     Length of the uni-dimensional matrix.
 * 
 * @tparam T      Type of the matrix elements.
*/
template<typename T>
void resetMatrix(T* matrix, const int len){
	for(int i = 0; i < len; i++)
		matrix[i] = 0.0;
}


/**
 * This function prints a matrix if a CSV file.
 * 
 * @param matrix  Matrix to print.
 * @param rows    Number of rows in the matrix.
 * @param cols    Number of columns in the matrix.
 * 
 * @tparam T      Type of the matrix elements.
*/
template<typename T>
void printMatrixCSV (const T* matrix, const size_t rows, const size_t cols) {
    const size_t dimension = rows * cols;
    std::ofstream file("result.csv");

    if (file.is_open()) {
        for (size_t i = 0; i < dimension; i++) {
            file << matrix[i];
            if((i + 1) % cols == 0)
                file << std::endl;
            else
              file << ',';
        }
        file.close();
    } 
    else
        std::cerr << "File not opened" << std::endl;
}



















