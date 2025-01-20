#pragma once

#include "texture_types.h"
#include <memory>

namespace rtx {

class AiInference {
public:
    virtual ~AiInference() = default;
    
    // Pure virtual interface for AI enhancement
    virtual TextureData enhanceTexture(const TextureData& input) = 0;
    virtual void setModel(const std::string& modelPath) = 0;
    virtual void setParameters(float upscaleFactor, float detailStrength) = 0;
};

class KuoniradAiInference : public AiInference {
public:
    static KuoniradAiInference& getInstance();
    
    TextureData enhanceTexture(const TextureData& input) override;
    void setModel(const std::string& modelPath) override;
    void setParameters(float upscaleFactor, float detailStrength) override;

private:
    KuoniradAiInference() = default;
    struct Implementation;
    std::unique_ptr<Implementation> impl_;
};

} // namespace rtx
