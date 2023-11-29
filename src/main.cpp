#include <iostream>
#include <typeinfo>
#include <benchmark/benchmark.h>
#include <omp.h>

#include "matrixMult.hpp"
#include "matrixManagement.hpp"

void testBench(auto* left, auto* right, auto* result, size_t rows, size_t inners, size_t columns, int unrollFactor, size_t tileSize);

/**
 * Initialise randomly two matrices and call functions performing matrix-matrix multiplication,
 * also doing Google Benchmark on them.
*/
int main (){

    int unrollFactor = 8;
	  size_t tileSize = 4;
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

      naiveMMM(left, right, result, rows, inners, columns);
      std::cout << std::endl << std::endl;
      
      float *result2 = new float[rows*columns];
      loopUnrollingMMM(left,right, result2, rows, inners, columns, unrollFactor);

      //ompMMM(left, right, result2, rows, inners, columns, tileSize /*TODO*/ );

      compareMatrix(rows, columns,result,result2);

      testBench(left,right,result,rows,inners,columns,unrollFactor,tileSize);
    }
    

    if(type == 'd'){
    	auto *left = generateMatrix<double>(rows, inners);
    	auto *right = generateMatrix<double>(inners, columns);
    	double *result = new double[rows * columns]; 

      naiveMMM(left, right, result, rows, inners, columns);
      std::cout << std::endl << std::endl;

      double *result2 = new double[rows*columns];
      loopUnrollingMMM(left,right, result2, rows, inners, columns, unrollFactor);

      //ompMMM(left, right, result2, rows, inners, columns, tileSize /*TODO*/ );

      compareMatrix(rows, columns,result,result2);

      testBench(left,right,result,rows,inners,columns,unrollFactor,tileSize);
    }

    return 0;
}

void testBench(auto* left, auto* right, auto* result, size_t rows, size_t inners, size_t columns, int unrollFactor, size_t tileSize) {


	benchmark::RegisterBenchmark("BM_naiveMMM", [&left, &right, &result, &rows, &inners, &columns](benchmark::State& state) {
      for (auto _ : state) {
        naiveMMM(left, right, result, rows, inners, columns);
      }
    });

      benchmark::RegisterBenchmark("BM_naiveAccMMM", [&left, &right, &result, &rows, &inners, &columns](benchmark::State& state) {
      for (auto _ : state) {
        naiveAccMMM(left, right, result, rows, inners, columns);
      }
    });

	 benchmark::RegisterBenchmark("BM_cacheFriendlyMMM", [&left, &right, &result, &rows, &inners, &columns](benchmark::State& state) {
      for (auto _ : state) {
        cacheFriendlyMMM(left, right, result, rows, inners, columns);
      }
    });


	benchmark::RegisterBenchmark("BM_tilingMMM", [&left, &right, &result, &rows, &inners, &columns, &tileSize](benchmark::State& state) {
      for (auto _ : state) {
        tilingMMM(left, right, result, rows, inners, columns, tileSize /*TODO*/ );
      }
    });
	
    
    benchmark::RegisterBenchmark("BM_ompMMM", [&left, &right, &result, &rows, &inners, &columns, &tileSize](benchmark::State& state) {
      for (auto _ : state) {
        ompMMM(left, right, result, rows, inners, columns, tileSize /*TODO*/ );
      }
    });
    
  
    benchmark::RegisterBenchmark("BM_loopUnrollingMMM", [&left, &right, &result, &rows, &inners, &columns, &unrollFactor](benchmark::State& state){
      for (auto _ : state) {
        loopUnrollingMMM(left,right, result, rows, inners, columns, unrollFactor);
      }
    });
   


    benchmark::RunSpecifiedBenchmarks();
    	
    //delete pointers
    delete[] left; delete[] right; delete[] result;
    
}
