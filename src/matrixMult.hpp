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
 * @param inners 
 * @param columns number of columns of the right matrix.
*/
template<typename T>
void naiveMMM(const T* left,const T* right, T* result, size_t rows, size_t inners, size_t columns) 
{
    for(size_t row = 0; row < rows; row++) {
        for(size_t col = 0; col < columns; col++) {
            for(size_t inner = 0; inner < inners; inner++) {
                result[row * columns] += 
                    left[inner * columns] * right[inner * columns + col];
            }
        }
    }
}


