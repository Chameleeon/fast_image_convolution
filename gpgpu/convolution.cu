#include <cuda_runtime.h>
#include <iostream>
#include <vector>

#define KERNEL_RADIUS 2
#define KERNEL_SIZE (2 * KERNEL_RADIUS + 1)
#define BLOCK_SIZE 16

__constant__ float d_kernel[KERNEL_SIZE * KERNEL_SIZE];

__global__ void convolutionKernel(const uint8_t *inputImage,
                                  uint8_t *outputImage, int width, int height,
                                  int bitCount) {
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;

  if (x >= width || y >= height) {
    return;
  }

  float sum[3] = {0.0f, 0.0f, 0.0f};
  for (int ky = -KERNEL_RADIUS; ky <= KERNEL_RADIUS; ky++) {
    for (int kx = -KERNEL_RADIUS; kx <= KERNEL_RADIUS; kx++) {
      int pixelX = min(max(x + kx, 0), width - 1);
      int pixelY = min(max(y + ky, 0), height - 1);
      int pixelIndex = (pixelY * width + pixelX) * 3;

      float pixel[3] = {inputImage[pixelIndex], inputImage[pixelIndex + 1],
                        inputImage[pixelIndex + 2]};

      float kernelValue =
          d_kernel[(ky + KERNEL_RADIUS) * KERNEL_SIZE + (kx + KERNEL_RADIUS)];
      for (int c = 0; c < 3; c++) {
        sum[c] += pixel[c] * kernelValue;
      }
    }
  }

  int outputIndex = (y * width + x) * 3;
  for (int c = 0; c < 3; c++) {
    outputImage[outputIndex + c] = static_cast<uint8_t>(sum[c]);
  }
}
