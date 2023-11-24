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
void naiveMMM(const T* left,const T* right, T* result, size_t rows, size_t inners, size_t columns) 
{
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
 * Computes the multiplication between two matrices by computing the inner 
 * product between the rows of the left matrix and the columns of the
 * right matrix. The inner product is performed in a variable
 * so that the update is done by accessing a register.
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
void naiveAccMMM(const T* left,const T* right, T* result, size_t rows, size_t inners, size_t columns) 
{
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
 * //todo
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
void cacheFriendlyMMM(const T* left,const T* right, T* result, size_t rows, size_t inners, size_t columns) 
{
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
 * //todo
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
void tilingMMM(const T* left,const T* right, T* result, size_t rows, size_t inners, size_t columns, size_t tileSize) 
{
    for(int innerTile = 0; innerTile < inners; innerTile += tileSize) {
        for(int row = 0; row < rows; row++) {
            int innerTileEnd = std::min(inners, innerTile + tileSize);
            for(int inner = innerTile; inner < innerTileEnd; inner++) {
                for(int column = 0; column < innerTileEnd; column++) {
                    result[row * columns + column] +=
                        left[row * inners + inner] * right[inner * columns + column];
                }
            }
        }
    }
    if(rows % tileSize != 0) {
        
    }
}