// #include "../../conv_lib/src/Convolution/Convolution.h"
// #include <chrono>
// #include <iostream>
//
// int main(int argc, char **argv) {
//   // EDGE DETECTION
//   // double arr[] = {0.0f, -1.0f, 0.0f, -1.0f, 4.0f, -1.0f, 0.0f, -1.0f,
//   0.0f};
//
//   // GAUSSIAN BLUR
//   double arr[] = {1.0f / 16, 2.0f / 16, 1.0f / 16, 2.0f / 16, 4.0f / 16,
//                   2.0f / 16, 1.0f / 16, 2.0f / 16, 1.0f / 16};
//   //
//   // BOX BLUR
//   // double arr[] = {1.0f / 9, 1.0f / 9, 1.0f / 9, 1.0f / 9, 1.0f / 9, 1.0f /
//   // 9, 1.0f / 9, 1.0f / 9, 1.0f / 9};
//   //
//   // SHARPEN
//   // double arr[] = {0.0f, -1.0f, 0.0f, -1.0f, 5.0f, -1.0f, 0.0f, -1.0f,
//   0.0f};
//
//   auto start = std::chrono::high_resolution_clock::now();
//   apply_convolution("test.bmp", 3, 3, arr, false);
//   auto end = std::chrono::high_resolution_clock::now();
//   auto duration =
//       std::chrono::duration_cast<std::chrono::microseconds>(end - start)
//           .count();
//
//   std::cout << "Time without parallelization: " << duration << "
//   microseconds."
//             << std::endl;
//
//   start = std::chrono::high_resolution_clock::now();
//   apply_convolution("test.bmp", 3, 3, arr, true);
//   end = std::chrono::high_resolution_clock::now();
//   duration = std::chrono::duration_cast<std::chrono::microseconds>(end -
//   start)
//                  .count();
//
//   std::cout << "Time with parallelization: " << duration << " microseconds."
//             << std::endl;
//
//   start = std::chrono::high_resolution_clock::now();
//   apply_convolution("test2.bmp", 3, 3, arr, false);
//   end = std::chrono::high_resolution_clock::now();
//   duration = std::chrono::duration_cast<std::chrono::microseconds>(end -
//   start)
//                  .count();
//
//   std::cout << "Time without parallelization: " << duration << "
//   microseconds."
//             << std::endl;
//
//   start = std::chrono::high_resolution_clock::now();
//   apply_convolution("test2.bmp", 3, 3, arr, true);
//   end = std::chrono::high_resolution_clock::now();
//   duration = std::chrono::duration_cast<std::chrono::microseconds>(end -
//   start)
//                  .count();
//
//   std::cout << "Time with parallelization: " << duration << " microseconds."
//             << std::endl;
//
//   return 0;
// }

#include <cstdint>
#include <iostream>
#include <vector>

#define KERNEL_RADIUS 2
#define KERNEL_SIZE (2 * KERNEL_RADIUS + 1)

extern "C" void apply_convolution_cuda(const uint8_t *inputImage,
                                       uint8_t *outputImage, int width,
                                       int height, const float *h_kernel);

int main() {
  // Load your image data into inputImage (not shown here)
  std::vector<uint8_t> inputImage(1024 * 768 *
                                  3); // Placeholder for input image data
  std::vector<uint8_t> outputImage(1024 * 768 *
                                   3); // Placeholder for output image data
  int width = 1024;                    // Image width
  int height = 768;                    // Image height

  float h_kernel[KERNEL_SIZE * KERNEL_SIZE] = {
      1.0 / 256, 4.0 / 256,  6.0 / 256,  4.0 / 256,  1.0 / 256,
      4.0 / 256, 16.0 / 256, 24.0 / 256, 16.0 / 256, 4.0 / 256,
      6.0 / 256, 24.0 / 256, 36.0 / 256, 24.0 / 256, 6.0 / 256,
      4.0 / 256, 16.0 / 256, 24.0 / 256, 16.0 / 256, 4.0 / 256,
      1.0 / 256, 4.0 / 256,  6.0 / 256,  4.0 / 256,  1.0 / 256};

  // Apply the convolution using CUDA
  apply_convolution_cuda(inputImage.data(), outputImage.data(), width, height,
                         h_kernel);

  // Save the output image (not shown here)
  return 0;
}
