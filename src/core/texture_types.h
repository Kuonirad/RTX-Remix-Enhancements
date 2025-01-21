#pragma once

#include <vector>
#include <memory>

namespace rtx {

/**
 * @brief A 4-component floating point vector used for color and texture data.
 * 
 * Represents RGBA color values or other 4D vector quantities in texture processing.
 * Components are accessible as xComponent, yComponent, zComponent, wComponent
 * and support basic arithmetic operations for efficient texture manipulation.
 * Each component represents a channel in RGBA color space or a dimension in 4D space.
 */
class Float4 {
private:
    float m_xComponent{0.0F};
    float m_yComponent{0.0F};
    float m_zComponent{0.0F};
    float m_wComponent{0.0F};

public:
    Float4() = default;
    explicit Float4(float value) 
        : m_xComponent(value)
        , m_yComponent(value)
        , m_zComponent(value)
        , m_wComponent(value) {}

    // Accessors
    [[nodiscard]] float xComponent() const { return m_xComponent; }
    [[nodiscard]] float yComponent() const { return m_yComponent; }
    [[nodiscard]] float zComponent() const { return m_zComponent; }
    [[nodiscard]] float wComponent() const { return m_wComponent; }

    // Mutators
    void setXComponent(float value) { m_xComponent = value; }
    void setYComponent(float value) { m_yComponent = value; }
    void setZComponent(float value) { m_zComponent = value; }
    void setWComponent(float value) { m_wComponent = value; }
    
    /**
     * @brief Multiplies all components by a scalar value.
     * 
     * @param scalar The value to multiply each component by
     * @return float4 A new vector with all components multiplied by scalar
     */
    Float4 operator*(float multiplier) const {
        Float4 result;
        result.setXComponent(m_xComponent * multiplier);
        result.setYComponent(m_yComponent * multiplier);
        result.setZComponent(m_zComponent * multiplier);
        result.setWComponent(m_wComponent * multiplier);
        return result;
    }
    
    Float4& operator+=(const Float4& other) {
        m_xComponent += other.xComponent();
        m_yComponent += other.yComponent();
        m_zComponent += other.zComponent();
        m_wComponent += other.wComponent();
        return *this;
    }
} __attribute__((aligned(alignof(float) * 4))); // Align to size of 4 floats

/**
 * @brief Container class for 2D texture data with RGBA color components.
 * 
 * Stores texture data as a contiguous array of float4 values, representing
 * RGBA pixels. Provides basic operations for texture manipulation including
 * resizing. The pixel data is stored in row-major order.
 */
class TextureData {
private:
    int m_width{0};
    int m_height{0};
    std::vector<Float4> m_pixels;

public:
    /**
     * @brief Gets the width of the texture.
     * @return The width in pixels
     */
    [[nodiscard]] int width() const { return m_width; }

    /**
     * @brief Gets the height of the texture.
     * @return The height in pixels
     */
    [[nodiscard]] int height() const { return m_height; }

    /**
     * @brief Gets a reference to the pixel array.
     * @return Reference to the pixel vector
     */
    [[nodiscard]] std::vector<Float4>& pixels() { return m_pixels; }
    [[nodiscard]] const std::vector<Float4>& pixels() const { return m_pixels; }

    /**
     * @brief Gets a reference to a pixel at the specified index.
     * @param index The linear index into the pixel array
     * @return Reference to the pixel data
     */
    [[nodiscard]] Float4& pixel(size_t index) { return m_pixels[index]; }
    [[nodiscard]] const Float4& pixel(size_t index) const { return m_pixels[index]; }

    /**
     * @brief Resizes the texture to the specified dimensions.
     * 
     * @param width New width of the texture in pixels
     * @param height New height of the texture in pixels
     * 
     * Reallocates the pixel buffer to accommodate the new dimensions.
     * Existing pixel data may be lost during resizing.
     */
    void resize(int width, int height) {
        m_width = width;
        m_height = height;
        m_pixels.resize(static_cast<size_t>(width) * static_cast<size_t>(height));
    }
};

} // namespace rtx
