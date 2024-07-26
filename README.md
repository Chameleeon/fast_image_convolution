# fast_image_convolution
A program written in C++ which tests performances of different types of optimizations on a simple image convolution algorithm.

## How to run:

### Requirements:
* premake5
* make
* gcc/g++

In order to run the program, you first need to give the #run.sh# script the permission to be executed:
```bash
sudo chmod +x run.sh
```

Then you can run the script and it will run all the tests and save the measurements to *measurements.csv*
```bash
./run.sh
```

## NOTE: Types of measurements:
* NO SIMD -> No SIMD optimizations have been applied
* SIMD -> Compiler SIMD optimizations have been applied
* CSIMD -> Custom SIMD optimizations have been applied

All of these are tested both parallelized and non-parallelized.
