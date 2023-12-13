#include <omp.h>

/**
 * This function performs matrix multiplication by iterating through each element of the resulting matrix
 * and calculating the dot product of the corresponding row in the left matrix and the corresponding column
 * in the right matrix. The result is stored in the provided result matrix.
 * 
 * @param left    Pointer to the left matrix.
 * @param right   Pointer to the right matrix.
 * @param result  Pointer to the result matrix.
 * @param rows    Number of rows in the left matrix.
 * @param inners  Number of columns in the left matrix and rows in the right matrix.
 * @param columns Number of columns in the right matrix.
 * 
 * @tparam T      Type of the matrix elements.
 */
template<typename T>
void naiveMMM (const T* left, const T* right, T* result, const size_t rows, const size_t inners, const size_t columns) {
    for(size_t row = 0; row < rows; row++)
        for(size_t col = 0; col < columns; col++) 
            for(size_t inner = 0; inner < inners; inner++) 
                result[row * columns + col] += 
                    left[row * columns + inner] * right[inner * columns + col];
}


/**
 * Computes the multiplication between two matrices, by computing the inner product between the rows of the left matrix 
 * and the columns of the right matrix. The inner product is performed in a variable so that the update is done 
 * by accessing a register.
 * 
 * @param left      Pointer to the first matrix.
 * @param right     Pointer to the second matrix.
 * @param result    Pointer to the resulting matrix.
 * @param rows      Number of rows in the matrices.
 * @param inners    Number of columns in the first matrix and rows in the second matrix.
 * @param columns   Number of columns in the matrices.
 * 
 * @tparam T        Type of the matrix elements.
*/
template<typename T>
void naiveAccMMM (const T* left, const T* right, T* result, const size_t rows, const size_t inners, const size_t columns) {
    for(size_t row = 0; row < rows; row++) 
        for(size_t col = 0; col < columns; col++) {
            T acc = T();
            for(size_t inner = 0; inner < inners; inner++)
                acc += left[row * columns + inner] * right[inner * columns + col];
            result[row * columns + col] = acc;
        }
}


/**
 * This function performs the matrix multiplication in a cache-friendly manner by accessing the elements of 
 * the matrices in a sequential manner. 
 * 
 * @param left      Pointer to the first matrix.
 * @param right     Pointer to the second matrix.
 * @param result    Pointer to the resulting matrix.
 * @param rows      Number of rows in the matrices.
 * @param inners    Number of columns in the first matrix and rows in the second matrix.
 * @param columns   Number of columns in the matrices.
 * 
 * @tparam T        Type of the matrix elements.
*/
template<typename T>
void cacheFriendlyMMM (const T* left, const T* right, T* result, const size_t rows, const size_t inners, const size_t columns) {
    for(size_t row = 0; row < rows; row++) 
        for(size_t inner = 0; inner < inners; inner++)
            for(size_t col = 0; col < columns; col++) 
                result[row * columns + col] += 
                    left[row * columns + inner] * right[inner * columns + col];
}


/**
 * This function performs the matrix multiplication using tiling optimization.
 * The tiling optimization divides the computation into smaller tiles to improve cache utilization.
 * The size of each tile is determined by the 'tileSize' template parameter.
 * 
 * @param left      Pointer to the left matrix.
 * @param right     Pointer to the right matrix.
 * @param result    Pointer to the result matrix.
 * @param rows      Number of rows in the matrices.
 * @param inners    Number of inner dimensions in the matrices.
 * @param columns   Number of columns in the matrices.
 * 
 * @tparam tileSize The size of each tile for tiling optimization.
 * @tparam T        Type of the matrix elements.     
 */
template<size_t tileSize, typename T>
void tilingMMM (const T* left, const T* right, T* result, const size_t rows, const size_t inners, const size_t columns) {
    for(size_t innerTile = 0; innerTile < inners; innerTile += tileSize) 
        for(size_t row = 0; row < rows; row++) {
            size_t innerTileEnd = std::min(inners, innerTile + tileSize);
            for(size_t inner = innerTile; inner < innerTileEnd; inner++) 
                for(size_t column = 0; column < columns; column++)
                    result[row * columns + column] += 
                        left[row * inners + inner] * right[inner * columns + column];
        }
}

/**
 * This function performs the matrix multiplication following the approach described in the 
 * naiveMMM tempate function, but optimizing it using OpenMP directives to achieve parallel
 * computation.
 *
 * @param left      Pointer to the first matrix.
 * @param right     Pointer to the second matrix.
 * @param result    Pointer to the resulting matrix.
 * @param rows      Number of rows in the matrices.
 * @param inners    Number of columns in the first matrix and rows in the second matrix.
 * @param columns   Number of columns in the matrices.
 * 
 * @tparam T        Type of the matrix elements.
*/
template<typename T>
void parallelMMM (const T* left, const T* right, T* result, const size_t rows, const size_t inners, const size_t columns) {
    #pragma omp parallel for default(shared) num_threads(8)
    for(size_t row = 0; row < rows; row++)
        for(size_t col = 0; col < columns; col++)
            for(size_t inner = 0; inner < inners; inner++)
                 result[row * columns + col] += 
                    left[row * columns + inner] * right[inner * columns + col];
}


/**
 * This function is a sort of combination of the previous methods performing the matrix matrix multiplication.
 * 
 * @param left      Pointer to the first matrix.
 * @param right     Pointer to the second matrix.
 * @param result    Pointer to the resulting matrix.
 * @param rows      Number of rows of the left matrix.
 * @param inners    Number of columns of the first matrix, equal to the number of rows of the second matrix.
 * @param columns   Number of columns of the second  matrix.
 * 
 * @tparam tileSize The size of each tile for tiling optimization.
 * @tparam T        Type of the matrix elements.
*/
template<size_t tileSize, typename T>
void highPerformanceMMM (const T* left, const T* right, T* result, const size_t rows, const size_t inners, const size_t columns) {
    #pragma omp parallel for simd shared(result, left, right, rows, inners, columns) default(none) num_threads(8) collapse(2)
    for (size_t rowTile = 0; rowTile < rows; rowTile += tileSize) 
        for (size_t columnTile = 0; columnTile < columns; columnTile += tileSize)
            for (size_t innerTile = 0; innerTile < inners; innerTile += tileSize) 
                for (size_t row = rowTile; row < rowTile + tileSize; row++) {
                    size_t innerTileEnd = std::min(inners, innerTile + tileSize);
                    for (size_t inner = innerTile; inner < innerTileEnd; inner++) 
                        for (size_t col = columnTile; col < columnTile + tileSize; col++) 
                            result[row * columns + col] += 
                                left[row * inners + inner] * right[inner * columns + col];
                }
}
