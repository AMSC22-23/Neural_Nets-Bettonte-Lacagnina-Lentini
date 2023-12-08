
INTRODUCTION 

The purpose of this project is to delve into the topic of matrix matrix multiplication, which is a highly relevant operation in many contexts.
Real-world applications often involve the use of large matrices, presenting a significant challenge for computational efficiency.
Machines must perform a high number of calculations, and the computation process can be slow and difficult.

For thess reasons, it is fondumental to manage large matrices and perform the multiplication between them in a intelligent way.
This means explore parallel programming, exploting specific libraries, and paying attention to the memory management of the system.

In this project, we explored variuos well-known techniques used in matrix matrix multiplication; we tested them individually, as well in combination, and at the end we analyzed the results and performance.
In particular, we utilized SIMD approch, loop unroll technique, OpenMp features and cache-friendly management.
To test them we used Google Benchmark library, and we saved the results in some reports file.
We exclusively concentrated on square matrices with elements of type float or double. 
We only considered the following dimensions:
     (16x16)
     (32x32)
     (64x64)
     (128x128)
     (512x512)
     (1024x1024)
     (2048x2048)

HOW TO COMPILE


HOW TO EXECUTE 


REPORT OF RESULTS
