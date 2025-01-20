#include "pde_solver.h"
#include <cmath>

namespace rtx {

void AnisotropicDiffusion::solve(TextureData& tex, int yStart, int yEnd, float dt) {
    const float k = 0.1f; // conductance parameter
    const int width = tex.width;
    
    for (int y = yStart; y < yEnd; y++) {
        for (int x = 0; x < width; x++) {
            const int idx = y * width + x;
            float4& pixel = tex.pixels[idx];
            
            // Compute gradients in 4 directions
            float4 north = (y > 0) ? tex.pixels[idx - width] : pixel;
            float4 south = (y < tex.height-1) ? tex.pixels[idx + width] : pixel;
            float4 east = (x < width-1) ? tex.pixels[idx + 1] : pixel;
            float4 west = (x > 0) ? tex.pixels[idx - 1] : pixel;
            
            // Compute diffusion coefficients
            float cn = computeConductionCoeff(std::abs(north.x - pixel.x));
            float cs = computeConductionCoeff(std::abs(south.x - pixel.x));
            float ce = computeConductionCoeff(std::abs(east.x - pixel.x));
            float cw = computeConductionCoeff(std::abs(west.x - pixel.x));
            
            // Update pixel values
            pixel.x += dt * (cn*(north.x - pixel.x) + cs*(south.x - pixel.x) +
                           ce*(east.x - pixel.x) + cw*(west.x - pixel.x));
            // Repeat for other channels (y,z,w)
        }
    }
}

float AnisotropicDiffusion::computeConductionCoeff(float gradient) {
    const float k = 0.1f;
    return std::exp(-(gradient*gradient)/(k*k));
}

void PoissonBlending::solve(TextureData& tex, int yStart, int yEnd, float dt) {
    const int width = tex.width;
    
    for (int y = yStart; y < yEnd; y++) {
        for (int x = 0; x < width; x++) {
            float laplacian;
            computeLaplacian(tex, x, y, laplacian);
            
            const int idx = y * width + x;
            tex.pixels[idx].x += dt * laplacian;
            // Repeat for other channels
        }
    }
}

void PoissonBlending::computeLaplacian(const TextureData& tex, int x, int y, float& result) {
    const int idx = y * tex.width + x;
    const float center = tex.pixels[idx].x;
    float sum = 0.0f;
    int count = 0;
    
    // 4-neighborhood Laplacian
    if (x > 0) { sum += tex.pixels[idx-1].x - center; count++; }
    if (x < tex.width-1) { sum += tex.pixels[idx+1].x - center; count++; }
    if (y > 0) { sum += tex.pixels[idx-tex.width].x - center; count++; }
    if (y < tex.height-1) { sum += tex.pixels[idx+tex.width].x - center; count++; }
    
    result = sum;
}

} // namespace rtx
