#include <iostream>
#include <typeinfo>

#include "matrixMult.hpp"
#include "matrixManagement.hpp"

/**
 * Initialise randomly two matrices and call functions performing matrix-matrix multiplication
*/
int main (){

 
    size_t rows, inners, columns;
    char type;
    
    //ask for rows, inners and columns (dimensions of matrices)
    std::cout << "Insert dimensions" << std::endl; 
    std::cout << "rows: " ; std::cin >> rows; 
    std::cout << "inners: " ; std::cin >> inners;
    std::cout << "columns: "; std::cin >> columns;
    
    //ask for type of elements (float or double)
    do{
    	std::cout << "Insert type of elements (f/d): "; 
    	std::cin >> type;
    }while(type !='f' && type != 'd');



    //allocate two uni-dimensional arrays and fill them randomly
    if(type == 'f'){
    	auto *left = generateMatrix<float>(rows, columns);
    	auto *right = generateMatrix<float>(inners, columns);
    	float *result = new float[rows * columns];
    	
    	//headMatrix(left, 5);
    	naiveMMM(left, right, result, rows, inners, columns);
    	
    	
    	//delete pointers
    	delete[] left; delete[] right; delete[] result;
    }
    

    if(type == 'd'){
    	auto *left = generateMatrix<double>(rows, inners);
    	auto *right = generateMatrix<double>(inners, columns);
    	double *result = new double[rows * columns]; 
    	
    	//headMatrix(left, 5);
    	naiveMMM(left, right, result, rows, inners, columns);
    	
    	//delete pointers
    	delete[] left; delete[] right; delete[] result;
    }

	
	
    
    
    return 0;
}
