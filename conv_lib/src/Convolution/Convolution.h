#include "../ImageUtil/BMP.h"

void apply_convolution(const std::string &filename, int k_width, int k_height,
                       double *kernel, bool use_parallel);
