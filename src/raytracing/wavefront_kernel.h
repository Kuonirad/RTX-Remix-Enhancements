#pragma once

#include "../core/texture_types.h"
#include <vector>

namespace rtx {

struct Ray {
    float origin[3];
    float direction[3];
    float tMin;
    float tMax;
} __attribute__((aligned(32)));

class ConcurrentWavefrontKernel {
public:
    static void generateRays(std::vector<Ray>& rays);
    void traceRays(const std::vector<Ray>& rays);
    static void shade(const std::vector<Ray>& rays);

private:
    void scheduleKernels();
    static void optimizeRayBatches(std::vector<Ray>& rays);
};

} // namespace rtx
