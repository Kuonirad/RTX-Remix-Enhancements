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
