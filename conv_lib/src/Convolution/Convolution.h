#include "../ImageUtil/BMP.h"
#include <vector>

void apply_convolution(const std::string &filename, int k_width, int k_height,
                       std::vector<double> kernel, bool use_parallel);

#ifdef CSIMD
void simd_convolution(const std::string &filename, int k_width, int k_height,
                      std::vector<double> kernel, bool use_parallel);
#endif
