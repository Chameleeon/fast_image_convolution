#include <cstdint>
#include <cuda_runtime.h>
#include <iostream>

#define KERNEL_RADIUS 2
#define KERNEL_SIZE (2 * KERNEL_RADIUS + 1)

__constant__ float d_kernel[KERNEL_SIZE * KERNEL_SIZE];

__global__ void convolutionKernel(const uint8_t *inputImage, uint8_t *outputImage, int width, int height, int bitCount) {
    // Kernel code here (you need to implement the actual convolution logic)
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < width && y < height) {
        // Example: copy input to output
        int idx = (y * width + x) * 3;
        outputImage[idx] = inputImage[idx];
        outputImage[idx + 1] = inputImage[idx + 1];
        outputImage[idx + 2] = inputImage[idx + 2];
    }
}

extern "C" void apply_convolution_cuda(const uint8_t *inputImage, uint8_t *outputImage, int width, int height, const float *h_kernel) {
    uint8_t *d_inputImage, *d_outputImage;
    size_t imageSize = width * height * 3 * sizeof(uint8_t);

    cudaMalloc(&d_inputImage, imageSize);
    cudaMalloc(&d_outputImage, imageSize);

    cudaMemcpy(d_inputImage, inputImage, imageSize, cudaMemcpyHostToDevice);
    cudaMemcpyToSymbol(d_kernel, h_kernel, KERNEL_SIZE * KERNEL_SIZE * sizeof(float));

    dim3 blockSize(16, 16);
    dim3 gridSize((width + blockSize.x - 1) / blockSize.x, (height + blockSize.y - 1) / blockSize.y);

    convolutionKernel<<<gridSize, blockSize>>>(d_inputImage, d_outputImage, width, height, 24);
    cudaDeviceSynchronize();

    cudaMemcpy(outputImage, d_outputImage, imageSize, cudaMemcpyDeviceToHost);

    cudaFree(d_inputImage);
    cudaFree(d_outputImage);
}
