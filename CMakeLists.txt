cmake_minimum_required(VERSION 3.20)

project(Neural_Nets_Bettonte_Lacagnina_Lentini)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

include_directories(.)

find_package(OpenMP REQUIRED)

find_package(OpenBLAS REQUIRED)

# Define a list of valid build types
set(VALID_BUILD_TYPES Debug Release RelWithDebInfo MinSizeRel)

# Set a default build type if none is specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Check if the specified build type is valid
list(FIND VALID_BUILD_TYPES "${CMAKE_BUILD_TYPE}" VALID_BUILD_TYPE_INDEX)
if(VALID_BUILD_TYPE_INDEX EQUAL -1)
    message(FATAL_ERROR "Invalid build type specified: ${CMAKE_BUILD_TYPE}. Allowed values are: ${VALID_BUILD_TYPES}")
endif()

# Print the current build type (optional)
message("--------------------------------------")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message("--------------------------------------")

# Not strictly necessary, CMake already sets some flags
if(CMAKE_BUILD_TYPE MATCHES "Release")
    add_compile_options(-Ofast -march=native) 
elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_compile_options(-O0 -g) 
endif()


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
