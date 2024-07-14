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

  file.seekg(fileHeader.offsetData, std::ios::beg);
  data.resize(fileHeader.fileSize - fileHeader.offsetData);
  file.read(reinterpret_cast<char *>(data.data()), data.size());

  file.close();
  std::cout << "BMP File Info:" << std::endl;
  std::cout << "Width: " << infoHeader.width << std::endl;
  std::cout << "Height: " << infoHeader.height << std::endl;
  std::cout << "Bit Count: " << infoHeader.bitCount << std::endl;
}

void BMP::writeBMP(const std::string &filename) {
  std::ofstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Could not open file: " << filename << std::endl;
    return;
  }

  file.write(reinterpret_cast<char *>(&fileHeader), sizeof(BMPFileHeader));
  file.write(reinterpret_cast<char *>(&infoHeader), sizeof(BMPInfoHeader));
  file.write(reinterpret_cast<char *>(data.data()), data.size());
  file.close();

  std::cout << "Written to file: " << filename << std::endl;
}
