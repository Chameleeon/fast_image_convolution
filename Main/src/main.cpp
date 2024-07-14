
#include "../../ConvLib/src/Convolution/Convolution.h"
// #include "ConvLib.h"
#include <iostream>

int main(int argc, char **argv) {
  // EDGE DETECTION
  // double arr[] = {0.0f, -1.0f, 0.0f, -1.0f, 4.0f, -1.0f, 0.0f, -1.0f, 0.0f};

  // GAUSSIAN BLUR
  double arr[] = {1.0f / 16, 2.0f / 16, 1.0f / 16, 2.0f / 16, 4.0f / 16,
                  2.0f / 16, 1.0f / 16, 2.0f / 16, 1.0f / 16};
  //
  // BOX BLUR
  // double arr[] = {1.0f / 9, 1.0f / 9, 1.0f / 9, 1.0f / 9, 1.0f / 9, 1.0f /
  // 9, 1.0f / 9, 1.0f / 9, 1.0f / 9};
  //
  // SHARPEN
  // double arr[] = {0.0f, -1.0f, 0.0f, -1.0f, 5.0f, -1.0f, 0.0f, -1.0f, 0.0f};

  apply_convolution("test.bmp", 3, 3, arr);
  return 0;
}
