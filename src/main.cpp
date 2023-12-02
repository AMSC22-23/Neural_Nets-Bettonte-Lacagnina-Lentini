#include <iostream>
#include <benchmark/benchmark.h>
#include <cblas.h>

#include "matrixMult.hpp"
#include "matrixManagement.hpp"

void testBench(auto* left, auto* right, auto* result, size_t rows, size_t inners, size_t columns, int unrollFactor, size_t tileSize, auto cblas);

/**
 * Initialise randomly two matrices and call functions performing matrix-matrix multiplication,
 * doing Google Benchmark on them.
*/
int main (){

    int unrollFactor = 8;
    size_t tileSize = 64;
    size_t rows, inners, columns;
    char type;
    
    //ask for rows, inners and columns (dimensions of matrices)
    std::cout << "Insert dimensions" << std::endl; 
    std::cout << "rows of the left matrix: " ; std::cin >> rows; 
    std::cout << "columns of the left matrix = (columns of the right one): " ;
    std::cin >> inners;
    std::cout << "columns of the right matrix: "; std::cin >> columns;

    //ask for type of elements (float or double)
    do {
    	std::cout << "Insert type of elements (f/d): "; 
    	std::cin >> type;
    } while (type !='f' && type != 'd');

    //allocate two uni-dimensional arrays and fill them randomly
    if (type == 'f') {
    	const float *left = generateMatrix<float>(rows, columns);
    	const float *right = generateMatrix<float>(inners, columns);
        float *result = new float[rows * columns];

        auto cblas = [](const float *A, const float *B, float *output, size_t rows_A, size_t inners_A_B, size_t cols_B) {
            cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, rows_A, cols_B, inners_A_B, 1.0f, A, rows_A, B, inners_A_B, 0.0f, output,rows_A);
        };

        //Check output
        cblas(left,right,result,rows,inners,columns);
        float *result = new float[rows*columns];
        naiveMMM(left,right,result2,rows,inners,columns);
        compareMatrix(rows,columns,result,result2);

        testBench(left,right,result,rows,inners,columns,unrollFactor,tileSize,cblas);
    }

    if (type == 'd') {
    	const double *left = generateMatrix<double>(rows, inners);
    	const double *right = generateMatrix<double>(inners, columns);
    	double *result = new double[rows * columns];

        auto cblas = [](const double *A, const double *B, double *output, size_t rows_A, size_t inners_A_B, size_t cols_B) {
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, rows_A, cols_B, inners_A_B, 1.0, A, rows_A, B, inners_A_B, 0.0, output,rows_A);
        };

        //Check output
        cblas(left,right,result,rows,inners,columns);
        double *result = new double[rows*columns];
        naiveMMM(left,right,result2,rows,inners,columns);
        compareMatrix(rows,columns,result,result2);

        testBench(left,right,result,rows,inners,columns,unrollFactor,tileSize,cblas);

    }

    return 0;
}

/**
 * Runs Google Benchmark to all implementations of matrix matrix multiplication;
 * for each implementation it defines a lambda function, each of them is called at the end of the function.
 * The benchmark is performed for each function once.
 * Before executing each implementation the resulting matrix is reset so that the correct result is obtained.
 *
 * @param left the left matrix.
 * @param right the right matrix.
 * @param result the resulting matrix.
 * @param rows number of rows of the left matrix.
 * @param inners number of columns of the left matrix, equal to the number of rows of the right matrix.
 * @param columns number of columns of the right matrix.
 * @param unrollFactor TODO
 * @param tileSize TODO
 * @param cblas lambda expression which contains float or double openBlas implementation.
*/
void testBench(auto* left, auto* right, auto* result, size_t rows, size_t inners, size_t columns, int unrollFactor, size_t tileSize, auto cblas) {

    benchmark::RegisterBenchmark("BM_naiveMMM", [&left, &right, &result, &rows, &inners, &columns](benchmark::State& state) {
        resetMatrix(result, rows * columns);
        for (auto _ : state)
            naiveMMM(left, right, result, rows, inners, columns);
    })->Iterations(1);


    benchmark::RegisterBenchmark("BM_naiveAccMMM", [&left, &right, &result, &rows, &inners, &columns](benchmark::State& state) {
        resetMatrix(result, rows * columns);
        for (auto _ : state)
            naiveAccMMM(left, right, result, rows, inners, columns);
    })->Iterations(1);


    benchmark::RegisterBenchmark("BM_cacheFriendlyMMM", [&left, &right, &result, &rows, &inners, &columns](benchmark::State& state) {
        resetMatrix(result, rows * columns);
        for (auto _ : state)
            cacheFriendlyMMM(left, right, result, rows, inners, columns);
    })->Iterations(1);


    benchmark::RegisterBenchmark("BM_loopUnrollingMMM", [&left, &right, &result, &rows, &inners, &columns, &unrollFactor](benchmark::State& state) {
        resetMatrix(result, rows * columns);
        for (auto _ : state)
            loopUnrollingMMM(left, right, result, rows, inners, columns, unrollFactor);
    })->Iterations(1);


    benchmark::RegisterBenchmark("BM_tilingMMM", [&left, &right, &result, &rows, &inners, &columns, &tileSize](benchmark::State& state) {
        resetMatrix(result, rows * columns);
        for (auto _ : state)
            tilingMMM(left, right, result, rows, inners, columns, tileSize);
    })->Iterations(1);


    benchmark::RegisterBenchmark("BM_parallelMMM", [&left, &right, &result, &rows, &inners, &columns](benchmark::State& state) {
        resetMatrix(result, rows * columns);
        for (auto _ : state)
            parallelMMM(left, right, result, rows, inners, columns);
    })->Iterations(1);


    benchmark::RegisterBenchmark("BM_highPerformanceMMM", [&left, &right, &result, &rows, &inners, &columns, &tileSize](benchmark::State& state) {
        resetMatrix(result, rows * columns);
        for (auto _ : state)
            highPerformanceMMM(left, right, result, rows, inners, columns, tileSize);
    })->Iterations(1);


    benchmark::RegisterBenchmark("BM_openBlasMMM", [&left, &right, &result, &rows, &inners, &columns, &cblas](benchmark::State& state) {
        resetMatrix(result, rows * columns);
        for (auto _ : state)
            cblas(left, right, result, rows, inners, columns);
    })->Iterations(1);


    //Run all lambda functions
    benchmark::RunSpecifiedBenchmarks();

    //delete pointers
    delete[] left;
    delete[] right;
    delete[] result;
}
