
//#define UNROLL_FACTOR 8
#include <omp.h>



/**
 * Computes the multiplication between two matrices by computing the inner 
 * product between the rows of the left matrix and the columns of the 
 * right matrix.
 * 
 * @tparam T the type of values contained in the matrices.
 * 
 * @param left the left matrix.
 * @param right the right matrix.
 * @param result the resulting matrix.
 * @param rows number of rows of the left matrix.
 * @param inners number of columns of the left matrix, equal to the number of rows of the right matrix.
 * @param columns number of columns of the right matrix.
*/
template<typename T>
void naiveMMM (const T* left,const T* right, T* result, size_t rows, size_t inners, size_t columns) {
    for(size_t row = 0; row < rows; row++) {
        for(size_t col = 0; col < columns; col++) {
            for(size_t inner = 0; inner < inners; inner++) {
                result[row * columns + col] += 
                    left[row * columns + inner] * right[inner * columns + col];
            }
        }
    }
}


/**
 * Computes the multiplication between two matrices,
 * by computing the inner product between the rows of the left matrix and the columns of the right matrix.
 * The inner product is performed in a variable so that the update is done by accessing a register.
 * 
 * @tparam T the type of values contained in the matrices.
 * 
 * @param left the left matrix.
 * @param right the right matrix.
 * @param result the resulting matrix.
 * @param rows number of rows of the left matrix.
 * @param inners number of columns of the left matrix, equal to the number of rows of the right matrix.
 * @param columns number of columns of the right matrix.
*/
template<typename T>
void naiveAccMMM (const T* left,const T* right, T* result, size_t rows, size_t inners, size_t columns) {
    for(size_t row = 0; row < rows; row++) {
        for(size_t col = 0; col < columns; col++) {
            T acc = T(); //T acc{};
            for(size_t inner = 0; inner < inners; inner++) {
                acc += left[row * columns + inner] * right[inner * columns + col];
            }
            result[row * columns + col] = acc;
        }
    }
}


/**
 * //TODO
 * 
 * @tparam T the type of values contained in the matrices.
 * 
 * @param left the left matrix.
 * @param right the right matrix.
 * @param result the resulting matrix.
 * @param rows number of rows of the left matrix.
 * @param inners number of columns of the left matrix, equal to the number of rows of the right matrix.
 * @param columns number of columns of the right matrix.
*/
template<typename T>
void cacheFriendlyMMM (const T* left,const T* right, T* result, size_t rows, size_t inners, size_t columns) {
    for(size_t row = 0; row < rows; row++) {
        for(size_t inner = 0; inner < inners; inner++) {
            for(size_t col = 0; col < columns; col++) {
                result[row * columns + col] += 
                    left[row * columns + inner] * right[inner * columns + col];
            }
        }
    }
}


 /**
 * TODO
 * @tparam T the type of values contained in the matrices.
 * 
 * @param left the left matrix.
 * @param right the right matrix.
 * @param result the resulting matrix.
 * @param rows number of rows of the left matrix.
 * @param inners number of columns of the left matrix, equal to the number of rows of the right matrix.
 * @param columns number of columns of the right matrix.
 * @param unrollFactor TODO
*/
template<typename T>
void loopUnrollingMMM (const T* left,const T* right, T* result, size_t rows, size_t inners, size_t columns, int unrollFactor) {
    size_t row;
    size_t col;
    size_t inner;
    

    for(row = 0; row < rows; row++) {
        for(col = 0; col < columns; col++) {
            #pragma GCC unroll 8
            for(inner = 0; inner < inners; inner++) {
                result[row * columns + col] += 
                    left[row * columns + inner] * right[inner * columns + col];
            }
        }
    }
    
}


/**
 * TODO
 * 
 * @tparam T the type of values contained in the matrices.
 * 
 * @param left the left matrix.
 * @param right the right matrix.
 * @param result the resulting matrix.
 * @param rows number of rows of the left matrix.
 * @param inners number of columns of the left matrix, equal to the number of rows of the right matrix.
 * @param columns number of columns of the right matrix.
 * @param tileSize TODO
*/
template<typename T>
void tilingMMM (const T* left,const T* right, T* result, size_t rows, size_t inners, size_t columns, size_t tileSize) {
    for(size_t innerTile = 0; innerTile < inners; innerTile += tileSize) {
        for(size_t row = 0; row < rows; row++) {
            size_t innerTileEnd = std::min(inners, innerTile + tileSize);
            for(size_t inner = innerTile; inner < innerTileEnd; inner++) {
                for(size_t column = 0; column < columns; column++) {
                    result[row * columns + column] += 
                        left[row * inners + inner] * right[inner * columns + column];
                }
            }
        }
    }
}

/**
 * TODO
 *
 * @tparam T the type of values contained in the matrices.
 * 
 * @param left the left matrix.
 * @param right the right matrix.
 * @param result the resulting matrix.
 * @param rows number of rows of the left matrix.
 * @param inners number of columns of the left matrix, equal to the number of rows of the right matrix.
*/
template<typename T>
void parallelMMM (const T* left,const T* right, T* result, size_t rows, size_t inners, size_t columns) {
    #pragma omp parallel for default(shared) num_threads(8)
        for(size_t row = 0; row < rows; row++) {
            for(size_t col = 0; col < columns; col++) {
                for(size_t inner = 0; inner < inners; inner++) {
                    result[row * columns + col] += 
                        left[row * columns + inner] * right[inner * columns + col];
                }
            }
    }
}


/**
 * TODO
 * 
 * @tparam T the type of values contained in the matrices.
 * 
 * @param left the left matrix.
 * @param right the right matrix.
 * @param result the resulting matrix.
 * @param rows number of rows of the left matrix.
 * @param inners number of columns of the left matrix, equal to the number of rows of the right matrix.
 * @param columns number of columns of the right matrix.
 * @param tileSize  TODO
*/
template<typename T>
void highPerformanceMMM (const T* left, const T* right, T* result, size_t rows, size_t inners, size_t columns, size_t tileSize) {
#pragma omp parallel for simd shared(result, left, right, rows, inners, columns, tileSize) default(none) \
	 collapse(2) num_threads(8)
  for (size_t rowTile = 0; rowTile < rows; rowTile += tileSize) {
    for (size_t columnTile = 0; columnTile < columns; columnTile += tileSize) {
        #pragma omp simd
      for (size_t innerTile = 0; innerTile < inners; innerTile += tileSize) {
            #pragma omp simd
        for (size_t row = rowTile; row < rowTile + tileSize; row++) {
          size_t innerTileEnd = std::min(inners, innerTile + tileSize);
          #pragma omp simd
          for (size_t inner = innerTile; inner < innerTileEnd; inner++) {
            #pragma omp simd
            for (size_t col = columnTile; col < columnTile + tileSize; col++) {
              result[row * columns + col] += 
                    left[row * inners + inner] * right[inner * columns + col];
            }

          }

        }

      }
      
    }

  }

}