#include "BMP.h"

BMP::BMP(const std::string &filename) : filename(filename) {}

void BMP::readBMP() {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Could not open file: " << filename << std::endl;
    return;
  }

  file.read(reinterpret_cast<char *>(&fileHeader), sizeof(BMPFileHeader));
  file.read(reinterpret_cast<char *>(&infoHeader), sizeof(BMPInfoHeader));

  int width = infoHeader.width;
  int height = infoHeader.height;
  int bitCount = infoHeader.bitCount;
  int rowSize =
      ((bitCount * width + 31) / 32) * 4; // Calculate row size with padding
  data.resize(rowSize * height);

  for (int y = 0; y < height; ++y) {
    file.read(reinterpret_cast<char *>(data.data() + y * rowSize), rowSize);
  }

  file.close();
}

void BMP::writeBMP(const std::string &filename) {
  std::ofstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Could not open file: " << filename << std::endl;
    return;
  }

  file.write(reinterpret_cast<char *>(&fileHeader), sizeof(BMPFileHeader));
  file.write(reinterpret_cast<char *>(&infoHeader), sizeof(BMPInfoHeader));

  int width = infoHeader.width;
  int height = infoHeader.height;
  int bitCount = infoHeader.bitCount;
  int rowSize =
      ((bitCount * width + 31) / 32) * 4; // Calculate row size with padding

  for (int y = 0; y < height; ++y) {
    file.write(reinterpret_cast<char *>(data.data() + y * rowSize), rowSize);
  }

  file.close();
  std::cout << "Written to file: " << filename << std::endl;
}
