#include "Convolution.h"
#include <algorithm>
#include <immintrin.h>
#include <omp.h>

template <typename T> T clamp(T value, T min, T max) {
  return std::max(min, std::min(value, max));
}

float get_pixel_value(const BMP &image, int width, int height, int x, int y,
                      int channel) {
  if (x < 0 || x >= width || y < 0 || y >= height)
    return 0.0f;
  int index = y * width + x;
  int byteIndex = index * 3 + channel;
  return (float)(image.data[byteIndex]) / 255.0f;
}

void set_pixel_value(BMP &output, int width, int x, int y, int channel,
                     float value) {
  int index = y * width + x;
  uint8_t byteValue = static_cast<uint8_t>(clamp(value * 255.0f, 0.0f, 255.0f));
  int byteIndex = index * 3 + channel;
  output.data[byteIndex] = byteValue;
}

void apply_convolution(const std::string &filename, int k_width, int k_height,
                       std::vector<double> kernel, bool use_parallel) {
  BMP image(filename);
  image.readBMP();

  int width = image.infoHeader.width;
  int height = image.infoHeader.height;
  int kernel_radius_x = k_width / 2;
  int kernel_radius_y = k_height / 2;

  BMP output = image;

#pragma omp parallel for if (use_parallel) collapse(2)
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      for (int channel = 0; channel < 3; channel++) {
        float value = 0.0f;
        for (int ky = 0; ky < k_height; ky++) {
          for (int kx = 0; kx < k_width; kx++) {
            int x = j + kx - kernel_radius_x;
            int y = i + ky - kernel_radius_y;
            value += get_pixel_value(image, width, height, x, y, channel) *
                     kernel[ky * k_width + kx];
          }
        }
        set_pixel_value(output, width, j, i, channel, value);
      }
    }
  }

  output.filename = filename + "_output.bmp";
  output.writeBMP(filename + "_output.bmp");
}

#ifdef CSIMD
void simd_convolution(const std::string &filename, int k_width, int k_height,
                      std::vector<double> kernel, bool use_parallel) {
  BMP image(filename);
  image.readBMP();

  int width = image.infoHeader.width;
  int height = image.infoHeader.height;
  int kernel_radius_x = k_width / 2;
  int kernel_radius_y = k_height / 2;

  BMP output = image;

  const int avx_floats = 256 / 32;

#pragma omp parallel for if (use_parallel) collapse(2)
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j += 8) {
      for (int channel = 0; channel < 3; channel++) {
        __m256 value = _mm256_setzero_ps();
        for (int ky = 0; ky < k_height; ky++) {
          for (int kx = 0; kx < k_width; kx++) {
            int x = j + kx - kernel_radius_x;
            int y = i + ky - kernel_radius_y;

            float pixels[8];

            for (int ind = 0; ind < avx_floats; ind++) {
              pixels[ind] =
                  get_pixel_value(image, width, height, x + ind, y, channel);
            }

            __m256 pixel_vec = _mm256_loadu_ps(pixels);
            __m256 kernel_vec =
                _mm256_set1_ps(static_cast<float>(kernel[ky * k_width + kx]));

            value = _mm256_add_ps(value, _mm256_mul_ps(pixel_vec, kernel_vec));
          }
        }

        float result[avx_floats];
        _mm256_storeu_ps(result, value);

        for (int v = 0; v < avx_floats; v++) {
          set_pixel_value(output, width, j + v, i, channel, result[v]);
        }
      }
    }
  }

  output.filename = "test_output.bmp";
  output.writeBMP("test_output.bmp");
}
#endif
