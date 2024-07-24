#include "../../conv_lib/src/Convolution/Convolution.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

#ifdef CSIMD
void convolve(const std::string &filename, int k_width, int k_height,
              std::vector<double> kernel, bool use_parallel) {
  simd_convolution(filename, k_width, k_height, kernel, use_parallel);
}
#else
void convolve(const std::string &filename, int k_width, int k_height,
              std::vector<double> kernel, bool use_parallel) {
  apply_convolution(filename, k_width, k_height, kernel, use_parallel);
}
#endif
#ifdef SIMD
std::string type = "SIMD";
#elif CSIMD
std::string type = "CSIMD";
#else
std::string type = "NO SIMD";
#endif

void log_measurement(const std::string &type, long long duration,
                     int num_pixels) {
  std::ofstream file("measurements.csv", std::ios::app);
  if (file.is_open()) {
    file << type << "," << duration << "," << num_pixels << "\n";
    file.close();
  } else {
    std::cerr << "Unable to open file for logging" << std::endl;
  }
}

std::vector<double> generate_gaussian_kernel(int size, double sigma) {
  int halfSize = size / 2;
  std::vector<double> kernel(size * size);
  double sum = 0.0;

  for (int y = -halfSize; y <= halfSize; ++y) {
    for (int x = -halfSize; x <= halfSize; ++x) {
      double value = exp(-(x * x + y * y) / (2 * sigma * sigma)) /
                     (2 * M_PI * sigma * sigma);
      kernel[(y + halfSize) * size + (x + halfSize)] = value;
      sum += value;
    }
  }

  for (double &value : kernel) {
    value /= sum;
  }

  return kernel;
}

int main(int argc, char **argv) {
  // EDGE DETECTION
  // double arr[] = {0.0f, -1.0f, 0.0f, -1.0f, 4.0f, -1.0f, 0.0f, -1.0f, 0.0f};

  // GAUSSIAN BLUR
  // double arr[] = {1.0 / 256, 4.0 / 256,  6.0 / 256,  4.0 / 256,  1.0 / 256,
  //                 4.0 / 256, 16.0 / 256, 24.0 / 256, 16.0 / 256, 4.0 / 256,
  //                 6.0 / 256, 24.0 / 256, 36.0 / 256, 24.0 / 256, 6.0 / 256,
  //                 4.0 / 256, 16.0 / 256, 24.0 / 256, 16.0 / 256, 4.0 / 256,
  //                 1.0 / 256, 4.0 / 256,  6.0 / 256,  4.0 / 256,  1.0 / 256};
  //
  // BOX BLUR
  // double arr[] = {1.0f / 9, 1.0f / 9, 1.0f / 9, 1.0f / 9, 1.0f / 9,
  //                 1.0f / 9, 1.0f / 9, 1.0f / 9, 1.0f / 9};
  //
  // SHARPEN
  // double arr[] = {0.0f, -1.0f, 0.0f, -1.0f, 5.0f, -1.0f, 0.0f, -1.0f,
  // 0.0f};
  int kernel_width = 15;
  int kernel_height = 15;
  std::vector<double> kernel = generate_gaussian_kernel(kernel_width, 1.0);

  convolve("test.bmp", kernel_width, kernel_height, kernel, false);
  convolve("test.bmp", kernel_width, kernel_height, kernel, false);
  convolve("test.bmp", kernel_width, kernel_height, kernel, false);

  auto start = std::chrono::high_resolution_clock::now();
  convolve("test.bmp", kernel_width, kernel_height, kernel, false);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();

  std::cout << "Time without parallelization: " << duration << " microseconds."
            << std::endl;
  log_measurement(type + ": Without Parallelization", duration, 1000);

  start = std::chrono::high_resolution_clock::now();
  convolve("test.bmp", kernel_width, kernel_height, kernel, true);
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start)
                 .count();

  std::cout << "Time with parallelization: " << duration << " microseconds."
            << std::endl;
  log_measurement(type + ": With Parallelization", duration, 1000);

  convolve("test2.bmp", kernel_width, kernel_height, kernel, false);
  convolve("test2.bmp", kernel_width, kernel_height, kernel, false);
  convolve("test2.bmp", kernel_width, kernel_height, kernel, false);

  start = std::chrono::high_resolution_clock::now();
  convolve("test2.bmp", kernel_width, kernel_height, kernel, false);
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start)
                 .count();

  std::cout << "Time without parallelization: " << duration << " microseconds."
            << std::endl;
  log_measurement(type + ": Without Parallelization", duration, 1000);

  start = std::chrono::high_resolution_clock::now();
  convolve("test2.bmp", kernel_width, kernel_height, kernel, true);
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start)
                 .count();

  std::cout << "Time with parallelization: " << duration << " microseconds."
            << std::endl;
  log_measurement(type + ": With Parallelization", duration, 1000);

  return 0;
}
