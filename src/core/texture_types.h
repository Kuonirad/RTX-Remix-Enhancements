#pragma once

#include <vector>
#include <memory>

namespace rtx {

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

class TextureData {
public:
    int width = 0;
    int height = 0;
    std::vector<float4> pixels;
    
    void resize(int w, int h) {
        width = w;
        height = h;
        pixels.resize(w * h);
    }
};

} // namespace rtx
