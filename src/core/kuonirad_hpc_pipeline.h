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
private:
    static constexpr size_t CACHE_LINE_SIZE = 64; // Modern CPU cache line size

public:
    // HPC concurrency toggles
    static const Option<bool> enablePipeline;
    static const Option<int> concurrencyThreads;
    
    // PDE configuration
    static const Option<PdeMethod> pdeMethod;
    static const Option<int> pdeIterations;
    static const Option<float> pdeTimestep;
    
    // Multi-resolution toggles
    static const Option<bool> useWavelet;
    
    // AI injection toggles
    static const Option<bool> enableAiRefinement;

    static void initialize();
    static TextureData runPipeline(const TextureData& input);

    struct PinnedTextureBuffer {
    private:
        std::vector<Float4> m_data;
        int m_width{0};
        int m_height{0};

    public:
        PinnedTextureBuffer() = default;
        
        [[nodiscard]] const std::vector<Float4>& data() const { return m_data; }
        std::vector<Float4>& data() { return m_data; }
        [[nodiscard]] int width() const { return m_width; }
        [[nodiscard]] int height() const { return m_height; }
        void setWidth(int width) { m_width = width; }
        void setHeight(int height) { m_height = height; }
        void resize(size_t size) { m_data.resize(size); }
    } __attribute__((aligned(CACHE_LINE_SIZE)));
    static PinnedTextureBuffer createPinnedBuffer(const TextureData& input);
    static void waveletDecompose(const std::vector<Float4>& inputData, int width, int height,
                               TextureData& lowFreq, TextureData& highFreq);
    static TextureData waveletRecompose(const TextureData& base, 
                                      const TextureData& detail);
};

} // namespace rtx
