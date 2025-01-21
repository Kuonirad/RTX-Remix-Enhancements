/*
 * Copyright (c) 2021-2024 Kuonirad RTX-Remix-Enhancements Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "kuonirad_hpc_pipeline.h"
#include "../core/ai_inference.h"
#include "../pde/pde_solver.h"
#include <thread>
#include <future>
#include <vector>
#include <memory>
#include <mutex>

namespace rtx {

KuoniradHpcPipeline::PinnedTextureBuffer KuoniradHpcPipeline::createPinnedBuffer(const TextureData& input) {
    PinnedTextureBuffer buf;
    buf.setWidth(input.width());
    buf.setHeight(input.height());
    buf.resize(static_cast<size_t>(buf.width()) * static_cast<size_t>(buf.height()));
    
    const size_t totalPixels = static_cast<size_t>(buf.width()) * static_cast<size_t>(buf.height());
    #pragma unroll 4
    for (size_t pixelIndex = 0; pixelIndex < totalPixels; pixelIndex++) {
        buf.data()[pixelIndex] = input.pixels()[pixelIndex];
    }
    return buf;
}

void KuoniradHpcPipeline::initialize() {
    // Initialize HPC concurrency or GPU streams here
}

TextureData KuoniradHpcPipeline::runPipeline(const TextureData& input) {
    if (!enablePipeline()) {
        return input;
    }

    // Create pinned memory buffer
    auto pinned = createPinnedBuffer(input);

    // Optional wavelet decomposition
    TextureData baseLevel;
    TextureData detailLevel;
    if (useWavelet()) {
        waveletDecompose(pinned.data(), pinned.width(), pinned.height(), baseLevel, detailLevel);
    } else {
        baseLevel = input;
    }

    // Domain decomposition for PDE passes
    static constexpr int MIN_TILE_HEIGHT = 1;  // Minimum height of a processing tile
    static constexpr int MIN_THREADS = 1;      // Minimum number of concurrent threads
    
    int numThreads = std::max(concurrencyThreads(), MIN_THREADS);
    int tileHeight = std::max(baseLevel.height() / numThreads, MIN_TILE_HEIGHT);
    std::vector<std::future<void>> futures;
    // Removed unused mutex

    for (int threadIndex = 0; threadIndex < numThreads; threadIndex++) {
        int yStart = threadIndex * tileHeight;
        int yEnd = (threadIndex == numThreads - 1) ? baseLevel.height() : (threadIndex + 1) * tileHeight;

        futures.push_back(std::async(std::launch::async, [&, yStart, yEnd]() {
            for (int iter = 0; iter < pdeIterations(); iter++) {
                switch (pdeMethod()) {
                    case PdeMethod::ANISOTROPIC_DIFFUSION:
                        AnisotropicDiffusion::solve(baseLevel, yStart, yEnd, pdeTimestep());
                        break;
                    case PdeMethod::POISSON_BLENDING:
                        PoissonBlending::solve(baseLevel, yStart, yEnd, pdeTimestep());
                        break;
                }
            }
        }));
    }

    for (auto& future : futures) {
        future.wait();
    }

    // Recombine if wavelet was used
    TextureData pdeOutput = baseLevel;
    if (useWavelet()) {
        pdeOutput = waveletRecompose(baseLevel, detailLevel);
    }

    // AI refinement pass
    TextureData finalOutput = pdeOutput;
    if (enableAiRefinement()) {
        finalOutput = KuoniradAiInference::getInstance().enhanceTexture(pdeOutput);
    }

    return finalOutput;
}

void KuoniradHpcPipeline::waveletDecompose(const std::vector<Float4>& inputData, int width, int height,
                                     TextureData& lowFreq, TextureData& highFreq) {
    lowFreq.resize(width, height);
    highFreq.resize(width, height);

    static constexpr float WAVELET_DECOMPOSE_FACTOR = 0.5F;  // Haar wavelet decomposition factor
    static constexpr float WAVELET_NORMALIZE_FACTOR = 1.0F;   // Normalization factor for wavelet transform
    
    const size_t totalPixels = static_cast<size_t>(width) * static_cast<size_t>(height);
    static constexpr float COMBINED_FACTOR = WAVELET_DECOMPOSE_FACTOR * WAVELET_NORMALIZE_FACTOR;
    
    #pragma unroll 4
    for (size_t pixelIndex = 0; pixelIndex < totalPixels; pixelIndex++) {
        const Float4& currentPixel = inputData[pixelIndex];
        lowFreq.pixels()[pixelIndex] = currentPixel * COMBINED_FACTOR;
        highFreq.pixels()[pixelIndex] = currentPixel * COMBINED_FACTOR;
    }
}

TextureData KuoniradHpcPipeline::waveletRecompose(const TextureData& base, const TextureData& detail) {
    static constexpr float RECOMPOSE_WEIGHT = 1.0F;  // Weight for detail contribution in recomposition
    
    TextureData output = base;
    const size_t totalPixels = static_cast<size_t>(base.width()) * static_cast<size_t>(base.height());
    
    #pragma unroll 4
    for (size_t pixelIndex = 0; pixelIndex < totalPixels; pixelIndex++) {
        output.pixels()[pixelIndex] += detail.pixels()[pixelIndex] * RECOMPOSE_WEIGHT;
    }
    return output;
}

} // namespace rtx
