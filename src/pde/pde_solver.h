#pragma once

#include "../core/texture_types.h"

namespace rtx {

class AnisotropicDiffusion {
public:
    /**
     * Solves the anisotropic diffusion equation for a texture region
     * @param texture The texture data to process
     * @param startRowIndex First row to process (inclusive)
     * @param endRowIndex Last row to process (exclusive)
     * @param deltaTime Time step for the diffusion process
     */
    static void solve(TextureData& texture, int startRowIndex, int endRowIndex, float deltaTime);

private:
    /**
     * Computes the conduction coefficient using Perona-Malik equation
     * @param gradientMagnitude Local intensity gradient magnitude
     * @param conductanceParameter Diffusion sensitivity parameter
     * @return Computed conduction coefficient
     */
    static float computeConductionCoeff(float gradientMagnitude, float conductanceParameter);
};

class PoissonBlending {
public:
    /**
     * Solves the Poisson equation for texture blending
     * @param texture The texture data to process
     * @param startRowIndex First row to process (inclusive)
     * @param endRowIndex Last row to process (exclusive)
     * @param deltaTime Time step for the blending process
     */
    static void solve(TextureData& texture, int startRowIndex, int endRowIndex, float deltaTime);

private:
    /**
     * Computes the discrete Laplacian at a pixel location
     * @param texture Source texture data
     * @param columnIndex X-coordinate of the target pixel
     * @param rowIndex Y-coordinate of the target pixel
     * @param laplacianResult Output parameter for the computed Laplacian
     */
    static void computeLaplacian(const TextureData& texture, int columnIndex, int rowIndex, float& laplacianResult);
};

} // namespace rtx
