
# Matrix multiplication on CPU

The purpose of this project is to delve into the topic of matrix matrix multiplication, which is a highly relevant operation in many contexts.
Real-world applications often involve the use of large matrices, which represent a significant challenge for computational efficiency.
Machines must perform a high number of calculations, and the computation process can be slow and difficult.

For these reasons, it is fondumental to manage large matrices and perform the multiplication between them in an intelligent way.
This means exploring parallel programming, exploting specific libraries, and paying attention to the memory management of the system.

In this project, we explored variuos well-known techniques used in matrix matrix multiplication; we analyzed them individually, as well in combination, and at the end we analyzed the results and performance; in particular, we utilized SIMD approach, loop unroll technique, OpenMp features and cache-friendly management.
To test them we used Google Benchmark library, and we saved the results in some reports file.

We exclusively concentrated on square matrices containing elements of type float or double. 

To obtain consistent results, the dimensions of the matrices should be a power of 2.

## Build with CMake

This project uses CMake as its build system, follow the steps below to build the project using it.

### Prerequisites

- CMake (version 3.20 or higher)
- OpenMP
- OpenBLAS

### Building the project

1. **Clone the repository**
   ```bash
   git clone https://github.com/AMSC22-23/Neural_Nets-Bettonte-Lacagnina-Lentini.git
   cd Neural_Nets-Bettone-Lacagnina-Lentini

2. **Create a Build Directory**
   ```bash
   mkdir build
   cd build

3. **Configure the project**

   To build with optimization flag (-OFast -march=native) you have to select this option
   ```bash
   cmake .. -DCMAKE_BUILD_TYPE=Release
   ```
   To build without optimization flag you have to select this option
    ```bash
   cmake .. -DCMAKE_BUILD_TYPE=Debug
    ```
    The default build is Release.
   
5. **Build the project**
   ```bash
   make

6. **Run the executable**
   ```bash
   ./Neural_Nets


## How to execute
Once the program is launched you will be asked to provide the following inputs:
- Dimension of the matrices.
- Type of matrices elements: insert **f** for float and **d** for double.
- Number of test repetitions.

The number of test repetitions will impact on the output of the program, in particular 1 repetition does not produce aggregate data (mean, value, standard deviation, cv).

To save the results produced by the benchmark, it is necessary to add the flag  `-benchmark_out=filename` when running the executable.
> [!NOTE]
> Save the output files in the **reports** directory in order to plot them with the [plotting.py](https://github.com/AMSC22-23/Neural_Nets-Bettonte-Lacagnina-Lentini/blob/main/src/plotting.py) script.


## Reporting results
The JSON files in the **reports** directory contain the results produced by Google Benchmark coming from our testing.
The naming convention we adopted is
`
reportXYZ.json
`,
where:
- **X** is the matrix dimension
- **Y** is an abbreviation for the type of data contained in the matrices (in this case **f** for float and **d** for double)
- **Z** could be the string **opt**, indicating tests run with optimization flags, or an empty string.

> [!WARNING]
> Using a different file naming or a different format is incompatible with the functioning of the plotting script.

To plot the data contained in the report files, type on the terminal in the `src` directory:
```
python3 plotting.py
```
The plots shown will also be saved in the **plots** directory.
> [!NOTE]
> It is necessary to have installed python3 and the matplotlib library to run the script.


## Contributors
- [Giorgio Bettonte](https://github.com/giorgiobettonte) 
- [Marco Lacagnina](https://github.com/marcolacagnina)
- [Fabio Lentini](https://github.com/fablnt)
