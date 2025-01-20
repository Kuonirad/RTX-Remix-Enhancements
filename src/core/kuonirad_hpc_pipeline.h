#pragma once

#include "rtx_option.h"
#include "texture_types.h"
#include <memory>

namespace rtx {

// This enum helps us switch PDE methods dynamically
enum class PdeMethod {
    ANISOTROPIC_DIFFUSION,
    POISSON_BLENDING
};

class KuoniradHpcPipeline {
public:
    // HPC concurrency toggles
    static Option<bool> enablePipeline;
    static Option<int> concurrencyThreads;
    
    // PDE configuration
    static Option<PdeMethod> pdeMethod;
    static Option<int> pdeIterations;
    static Option<float> pdeTimestep;
    
    // Multi-resolution toggles
    static Option<bool> useWavelet;
    
    // AI injection toggles
    static Option<bool> enableAiRefinement;

    static void initialize();
    static TextureData runPipeline(const TextureData& input);

private:
    struct PinnedTextureBuffer {
        std::unique_ptr<float4[]> data;
        int width;
        int height;
    };

    static PinnedTextureBuffer createPinnedBuffer(const TextureData& input);
    static void waveletDecompose(const float4* inputData, int width, int height,
                               TextureData& lowFreq, TextureData& highFreq);
    static TextureData waveletRecompose(const TextureData& base, 
                                      const TextureData& detail);
};

} // namespace rtx
