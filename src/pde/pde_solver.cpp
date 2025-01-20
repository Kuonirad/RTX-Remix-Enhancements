#include "pde_solver.h"
#include <cmath>   // for std::exp, std::abs

namespace rtx {

void AnisotropicDiffusion::solve(TextureData& texture, int startRowIndex, int endRowIndex, float deltaTime) {
    // Perona-Malik diffusion parameters
    static constexpr float DEFAULT_CONDUCTANCE = 0.1F;     // Default conductance (k) for edge-preserving diffusion
    static constexpr int VON_NEUMANN_SIZE = 4;            // 4-directional von Neumann neighborhood
    static constexpr float GRADIENT_SCALE = 1.0F;         // Scaling factor for gradient contribution
    static constexpr float MIN_GRADIENT = 0.0F;           // Minimum gradient threshold
    static constexpr float MAX_GRADIENT = 1.0F;           // Maximum gradient threshold

    const int width = texture.width();
    
    #pragma unroll 4
    for (int currentRow = startRowIndex; currentRow < endRowIndex; currentRow++) {
        #pragma unroll 4
        for (int currentColumn = 0; currentColumn < width; currentColumn++) {
            const size_t pixelIndex = static_cast<size_t>(currentRow) * static_cast<size_t>(width) + static_cast<size_t>(currentColumn);
            Float4& currentPixel = texture.pixels()[pixelIndex];
            
            // Compute gradients in NEIGHBORHOOD_SIZE directions
            Float4 northPixel = (currentRow > 0) ? texture.pixels()[pixelIndex - width] : currentPixel;
            Float4 southPixel = (currentRow < texture.height()-1) ? texture.pixels()[pixelIndex + width] : currentPixel;
            Float4 eastPixel = (currentColumn < width-1) ? texture.pixels()[pixelIndex + 1] : currentPixel;
            Float4 westPixel = (currentColumn > 0) ? texture.pixels()[pixelIndex - 1] : currentPixel;
            
            // Compute diffusion coefficients using Perona-Malik conductance function
            // Compute directional conduction coefficients using Perona-Malik equation
            float northConductance = computeConductionCoeff(
                std::clamp(std::abs(northPixel.xComponent() - currentPixel.xComponent()), MIN_GRADIENT, MAX_GRADIENT),
                DEFAULT_CONDUCTANCE);
            float southConductance = computeConductionCoeff(
                std::clamp(std::abs(southPixel.xComponent() - currentPixel.xComponent()), MIN_GRADIENT, MAX_GRADIENT),
                DEFAULT_CONDUCTANCE);
            float eastConductance = computeConductionCoeff(
                std::clamp(std::abs(eastPixel.xComponent() - currentPixel.xComponent()), MIN_GRADIENT, MAX_GRADIENT),
                DEFAULT_CONDUCTANCE);
            float westConductance = computeConductionCoeff(
                std::clamp(std::abs(westPixel.xComponent() - currentPixel.xComponent()), MIN_GRADIENT, MAX_GRADIENT),
                DEFAULT_CONDUCTANCE);
            
            // Update pixel values using weighted gradients from all directions
            currentPixel.setXComponent(currentPixel.xComponent() + deltaTime * GRADIENT_SCALE * (
                northConductance*(northPixel.xComponent() - currentPixel.xComponent()) + 
                southConductance*(southPixel.xComponent() - currentPixel.xComponent()) +
                eastConductance*(eastPixel.xComponent() - currentPixel.xComponent()) + 
                westConductance*(westPixel.xComponent() - currentPixel.xComponent())) / 
                static_cast<float>(VON_NEUMANN_SIZE));
            // Repeat for other channels (y,z,w)
        }
    }
}

float AnisotropicDiffusion::computeConductionCoeff(float gradientMagnitude, float conductanceParameter) {
    const float squared_gradient = gradientMagnitude * gradientMagnitude;
    const float squared_conductance = conductanceParameter * conductanceParameter;
    return std::exp(-squared_gradient / squared_conductance);
}

void PoissonBlending::solve(TextureData& texture, int startRowIndex, int endRowIndex, float deltaTime) {
    // Constants for Poisson equation solver
    static constexpr float ZERO_LAPLACIAN = 0.0F;          // Initial value for Laplacian accumulator
    static constexpr float BLEND_WEIGHT = 1.0F;            // Weight for Laplacian contribution in blending
    const int width = texture.width();
    auto& pixels = texture.pixels();
    
    #pragma unroll 4
    for (int currentRow = startRowIndex; currentRow < endRowIndex; currentRow++) {
        #pragma unroll 4
        for (int currentColumn = 0; currentColumn < width; currentColumn++) {
            float laplacianValue = ZERO_LAPLACIAN;
            computeLaplacian(texture, currentColumn, currentRow, laplacianValue);
            
            const size_t pixelIndex = static_cast<size_t>(currentRow) * static_cast<size_t>(width) + static_cast<size_t>(currentColumn);
            const float currentValue = pixels[pixelIndex].xComponent();
            const float blendedValue = currentValue + deltaTime * BLEND_WEIGHT * laplacianValue;
            pixels[pixelIndex].setXComponent(blendedValue);
            // Repeat for other channels
        }
    }
}

void PoissonBlending::computeLaplacian(const TextureData& texture, int columnIndex, int rowIndex, float& laplacianResult) {
    // Constants for discrete Laplacian computation
    static constexpr float ZERO_GRADIENT = 0.0F;         // Initial value for gradient accumulator
    static constexpr int MIN_BOUNDARY = 0;              // Minimum array index for boundary checks

    const size_t currentPixelIndex = static_cast<size_t>(rowIndex) * static_cast<size_t>(texture.width()) + static_cast<size_t>(columnIndex);
    const float centerPixelValue = texture.pixels()[currentPixelIndex].xComponent();
    float gradientSum = ZERO_GRADIENT;
    int neighborCount = 0;
    
    // Discrete Laplacian using 4-point stencil (von Neumann neighborhood)
    // Check west neighbor
    if (columnIndex > MIN_BOUNDARY) { 
        const float westGradient = texture.pixels()[currentPixelIndex-1].xComponent() - centerPixelValue;
        gradientSum += westGradient;
        neighborCount++; 
    }
    // Check east neighbor
    if (columnIndex < texture.width()-1) { 
        const float eastGradient = texture.pixels()[currentPixelIndex+1].xComponent() - centerPixelValue;
        gradientSum += eastGradient;
        neighborCount++; 
    }
    // Check north neighbor
    if (rowIndex > MIN_BOUNDARY) { 
        const float northGradient = texture.pixels()[currentPixelIndex-texture.width()].xComponent() - centerPixelValue;
        gradientSum += northGradient;
        neighborCount++; 
    }
    // Check south neighbor
    if (rowIndex < texture.height()-1) { 
        const float southGradient = texture.pixels()[currentPixelIndex+texture.width()].xComponent() - centerPixelValue;
        gradientSum += southGradient;
        neighborCount++; 
    }
    
    // Normalize by actual number of neighbors for boundary pixels
    // Use VON_NEUMANN_SIZE for normalization
    static constexpr int VON_NEUMANN_SIZE = 4;  // 4-directional von Neumann neighborhood
    laplacianResult = (neighborCount == VON_NEUMANN_SIZE) ? 
        gradientSum : 
        (gradientSum * static_cast<float>(VON_NEUMANN_SIZE) / static_cast<float>(neighborCount));
}

} // namespace rtx
