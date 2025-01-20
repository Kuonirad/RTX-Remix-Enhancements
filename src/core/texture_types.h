#pragma once

#include <vector>
#include <memory>

namespace rtx {

/**
 * @brief A 4-component floating point vector used for color and texture data.
 * 
 * Represents RGBA color values or other 4D vector quantities in texture processing.
 * Components are accessible as x, y, z, w and support basic arithmetic operations
 * for efficient texture manipulation.
 */
struct float4 {
    float x, y, z, w;
    
    float4() : x(0), y(0), z(0), w(0) {}
    float4(float v) : x(v), y(v), z(v), w(v) {}
    
    float4 operator*(float scalar) const {
        float4 result;
        result.x = x * scalar;
        result.y = y * scalar;
        result.z = z * scalar;
        result.w = w * scalar;
        return result;
    }
    
    float4& operator+=(const float4& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }
};

/**
 * @brief Container class for 2D texture data with RGBA color components.
 * 
 * Stores texture data as a contiguous array of float4 values, representing
 * RGBA pixels. Provides basic operations for texture manipulation including
 * resizing. The pixel data is stored in row-major order.
 */
class TextureData {
public:
    int width = 0;
    int height = 0;
    std::vector<float4> pixels;
    
    /**
     * @brief Resizes the texture to the specified dimensions.
     * 
     * @param w New width of the texture in pixels
     * @param h New height of the texture in pixels
     * 
     * Reallocates the pixel buffer to accommodate the new dimensions.
     * Existing pixel data may be lost during resizing.
     */
    void resize(int w, int h) {
        width = w;
        height = h;
        pixels.resize(w * h);
    }
};

} // namespace rtx
