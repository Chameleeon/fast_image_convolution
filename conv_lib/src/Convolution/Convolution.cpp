#include "Convolution.h"
#include <algorithm>
#include <omp.h>

template <typename T>
T clamp(T value, T min, T max) {
    return std::max(min, std::min(value, max));
}

void apply_convolution(const std::string &filename, int k_width, int k_height, double *kernel, bool use_parallel) {
    BMP image(filename);
    image.readBMP();

    int width = image.infoHeader.width;
    int height = image.infoHeader.height;
    int bit_count = image.infoHeader.bitCount;
    int kernel_radius_x = k_width / 2;
    int kernel_radius_y = k_height / 2;

    BMP output = image;

    // normalize pixel values
    auto get_pixel_value = [&](int x, int y, int channel) -> float {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return 0.0f;
        int index = y * width + x;
        if (bit_count == 32) {
            uint32_t pixel = *reinterpret_cast<uint32_t *>(&image.data[index * 4]);
            return (float)((pixel >> (channel * 8)) & 0xFF) / 255.0f;
        } else if (bit_count == 24) {
            int byteIndex = index * 3 + channel;
            return (float)(image.data[byteIndex]) / 255.0f;
        } else if (bit_count == 16) {
            uint16_t pixel = *reinterpret_cast<uint16_t *>(&image.data[index * 2]);
            if (channel == 0)
                return ((pixel & 0x1F) << 3) / 255.0f;
            if (channel == 1)
                return (((pixel >> 5) & 0x3F) << 2) / 255.0f;
            if (channel == 2)
                return (((pixel >> 11) & 0x1F) << 3) / 255.0f;
        } else if (bit_count == 8) {
            return image.data[index] / 255.0f;
        }
        return 0.0f;
    };

    auto set_pixel_value = [&](int x, int y, int channel, float value) {
        int index = y * width + x;
        uint8_t byteValue = static_cast<uint8_t>(clamp(value * 255.0f, 0.0f, 255.0f));

        if (bit_count == 32) {
            uint32_t &pixel = *reinterpret_cast<uint32_t *>(&output.data[index * 4]);
            pixel = (pixel & ~(0xFF << (channel * 8))) | (byteValue << (channel * 8));
        } else if (bit_count == 24) {
            int byteIndex = index * 3 + channel;
            output.data[byteIndex] = byteValue;
        } else if (bit_count == 16) {
            uint16_t &pixel = *reinterpret_cast<uint16_t *>(&output.data[index * 2]);
            if (channel == 0)
                pixel = (pixel & ~0x1F) | (byteValue >> 3);
            if (channel == 1)
                pixel = (pixel & ~(0x3F << 5)) | ((byteValue >> 2) << 5);
            if (channel == 2)
                pixel = (pixel & ~(0x1F << 11)) | ((byteValue >> 3) << 11);
        } else if (bit_count == 8) {
            output.data[index] = byteValue;
        }
    };

#pragma omp parallel for if (use_parallel) collapse(2)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int channels = bit_count < 32 ? 3 : 4;
            for (int channel = 0; channel < channels; channel++) {
                float value = 0.0f;
                for (int ky = 0; ky < k_height; ky++) {
                    for (int kx = 0; kx < k_width; kx++) {
                        int x = j + kx - kernel_radius_x;
                        int y = i + ky - kernel_radius_y;
                        value += get_pixel_value(x, y, channel) * kernel[ky * k_width + kx];
                    }
                }
                set_pixel_value(j, i, channel, value);
            }
        }
    }
    output.filename = filename + "_output.bmp";
    output.writeBMP(filename + "_output.bmp");
}
