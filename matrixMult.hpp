template<typename T>
void naiveMMM(T* left, T* right, T* result, size_t rows, size_t inners, size_t columns) 
{
    for(size_t row = 0; i < rows; i++) {
        for(size_t col = 0; j < columns) {
            for(size_t inner = 0; k < inners; k++) {
                result[row * columns] += 
                    left[inner * columns] * right[inner * columns + col];
            }
        }
    }
}


