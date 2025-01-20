#pragma once

#include "../core/texture_types.h"
#include <string>

namespace rtx {

class UsdIntegration {
public:
    static void initialize(const std::string& connectionUrl);
    static void syncTextureChanges(const TextureData& updatedTexture, const std::string& texturePath);
    static void handleExternalUpdates();
    static void broadcastChanges(const std::string& assetPath);
    static void subscribeToChanges(const std::string& assetPath);
    static void setupRoundTripWorkflow();
    static void validateUsdSchema();
};

} // namespace rtx
