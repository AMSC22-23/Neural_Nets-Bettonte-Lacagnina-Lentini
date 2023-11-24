

/**
 * generate two uni-dimensional arrays of the specified type (float or double) and dimensions
 * @param rows, inners, cols for dimensions of arrays
 * @param type specifies type of elements
*/

#include <iostream>
#include <random>
#include <chrono>

template<typename T>
T* generateMatrix(size_t rows, size_t cols) 
{ 
	auto currentTime = std::chrono::high_resolution_clock::now();
	int dimension = rows * cols;
    unsigned int seed = static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count());
    
    std::mt19937 gen(seed); // Generator based on Mersenne Twister
    std::uniform_real_distribution<T> distribution(0.0, 100000.0); // uniformal distribution between 0 and 100k
	
	T* matrix = new T[dimension];
	for (int i = 0; i < dimension; ++i) {
        matrix[i] = distribution(gen);
    }

    return matrix;
}

template<typename T>
void headMatrix(T* matrix, int index){
	int i;
	for (int i = 0; i < index; ++i) {
		std::cout << matrix[i] << "  ";
    }
}
