cmake_minimum_required(VERSION 3.20)

project(Neural_Nets_Bettonte_Lacagnina_Lentini)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

include_directories(.)

find_package(OpenMP REQUIRED)

find_package(OpenBLAS REQUIRED)

set(CMAKE_CXX_FLAGS "-O3 -ffast-math -march=native -Wall -Werror -Wpedantic -Wcast-align -Wshadow -Wpointer-arith -Wcast-qual -Wno-missing-braces -fconcepts -fopenmp -Weffc++ -Wunused-variable -fsanitize=address -fsanitize=leak -Wunreachable-code")

#flag da proporre
#-Weffc++ avvisa su pratiche non conformi all effective c++
#-Wunused-variable  -Wunused-parameter //per parametri e variabili non utilizzate
#-fsanitize=address  -fsanitize=leak
#-Wunreachable-code 

# To include GoogleBenchmark
include(FetchContent)
set(BENCHMARK_ENABLE_TESTING off)    # to suppress benchmark internal tests
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0)      # or "main" for latest
FetchContent_Declare(googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.6.0)              # or "main" for latest
FetchContent_MakeAvailable(googletest googlebenchmark)

add_executable(Neural_nets src/main.cpp)

target_link_libraries(Neural_nets PRIVATE benchmark::benchmark)
target_link_libraries(Neural_nets PRIVATE OpenMP::OpenMP_CXX)


if(OpenBLAS_FOUND)
    message("OpenBLAS found")
    include_directories(${OpenBLAS_INCLUDE_DIRS})
    target_link_libraries(Neural_nets PRIVATE ${OpenBLAS_LIBRARIES})
else()
    message(FATAL_ERROR "OpenBLAS not found")
endif()