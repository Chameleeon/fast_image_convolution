#include "../ImageUtil/BMP.h"
#include <vector>

#if defined(SIMD) || defined(NOSIMD)
void apply_convolution(const std::string &filename, int k_width, int k_height,
                       std::vector<double> kernel, bool use_parallel);
#endif

#ifdef CSIMD
void simd_convolution(const std::string &filename, int k_width, int k_height,
                      std::vector<double> kernel, bool use_parallel);
#endif
