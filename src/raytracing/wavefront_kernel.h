#pragma once

#include "../core/texture_types.h"
#include <vector>

namespace rtx {

struct Ray {
    float origin[3];
    float direction[3];
    float tMin;
    float tMax;
};

class ConcurrentWavefrontKernel {
public:
    void generateRays(std::vector<Ray>& rays);
    void traceRays(const std::vector<Ray>& rays);
    void shade(const std::vector<Ray>& rays);

private:
    void scheduleKernels();
    void optimizeRayBatches(std::vector<Ray>& rays);
};

} // namespace rtx
