#include "../../conv_lib/src/Convolution/Convolution.h"
#include <chrono>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <vector>

#ifdef CSIMD
void convolve(const std::string &filename, int k_width, int k_height,
              const std::vector<double> &kernel, bool use_parallel) {
  simd_convolution(filename, k_width, k_height, kernel, use_parallel);
}
#elif defined(SIMD) || defined(NOSIMD)
void convolve(const std::string &filename, int k_width, int k_height,
              const std::vector<double> &kernel, bool use_parallel) {
  apply_convolution(filename, k_width, k_height, kernel, use_parallel);
}
#else
void convolve(const std::string &filename, int k_width, int k_height,
              const std::vector<double> &kernel, bool use_parallel) {
  std::cout << "Unsupported Build Configuration!" << std::endl;
}
#endif

#ifdef SIMD
std::string type = "SIMD";
#elif CSIMD
std::string type = "CSIMD";
#elif NOSIMD
std::string type = "NO SIMD";
#endif

void log_measurement(const std::string &type, long long duration,
                     const std::string &filename) {
  std::ofstream file("measurements.csv", std::ios::app);
  if (file.is_open()) {
    file << filename << "," << type << "," << duration << "\n";
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
  int kernel_width = 3;
  int kernel_height = 3;
  std::vector<double> kernel = generate_gaussian_kernel(kernel_width, 1.0);

  for (const auto &entry : std::filesystem::directory_iterator("images")) {
    if (entry.path().extension() == ".bmp") {
      const std::string &filename = entry.path().string();

      std::cout << "Processing file: " << filename << std::endl;
      std::cout << "Warming up..." << std::endl;
      convolve(filename, kernel_width, kernel_height, kernel, false);
      convolve(filename, kernel_width, kernel_height, kernel, false);

      auto start = std::chrono::high_resolution_clock::now();
      convolve(filename, kernel_width, kernel_height, kernel, false);
      auto end = std::chrono::high_resolution_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::microseconds>(end - start)
              .count();

      std::cout << "Time without parallelization for " << filename << ": "
                << duration << " microseconds." << std::endl;
      log_measurement(type + ": Without Parallelization", duration, filename);

      start = std::chrono::high_resolution_clock::now();
      convolve(filename, kernel_width, kernel_height, kernel, true);
      end = std::chrono::high_resolution_clock::now();
      duration =
          std::chrono::duration_cast<std::chrono::microseconds>(end - start)
              .count();

      std::cout << "Time with parallelization for " << filename << ": "
                << duration << " microseconds." << std::endl;
      log_measurement(type + ": With Parallelization", duration, filename);
    }
  }

  return 0;
}
