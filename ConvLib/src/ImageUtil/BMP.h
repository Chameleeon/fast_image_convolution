#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#pragma pack(push, 1)

struct BMPFileHeader {
  uint16_t fileType{0x4D42};
  uint32_t fileSize{0};
  uint16_t reserved1{0};
  uint16_t reserved2{0};
  uint32_t offsetData{0};
};

struct BMPInfoHeader {
  uint32_t size{0};
  int32_t width{0};
  int32_t height{0};
  uint16_t planes{1};
  uint16_t bitCount{0};
  uint32_t compression{0};
  uint32_t sizeImage{0};
  int32_t xPixelsPerMeter{0};
  int32_t yPixelsPerMeter{0};
  uint32_t colorsUsed{0};
  uint32_t colorsImportant{0};
};

#pragma pack(pop)

class BMP {
public:
  BMP(const std::string &filename);
  void readBMP();
  void writeBMP(const std::string &filename);
  std::string filename;
  BMPFileHeader fileHeader;
  BMPInfoHeader infoHeader;
  std::vector<uint8_t> data;
};
