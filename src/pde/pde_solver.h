#pragma once

#include "../core/texture_types.h"

namespace rtx {

class AnisotropicDiffusion {
public:
    static void solve(TextureData& tex, int yStart, int yEnd, float dt);

private:
    static float computeConductionCoeff(float gradient);
};

class PoissonBlending {
public:
    static void solve(TextureData& tex, int yStart, int yEnd, float dt);

private:
    static void computeLaplacian(const TextureData& tex, int x, int y, float& result);
};

} // namespace rtx
