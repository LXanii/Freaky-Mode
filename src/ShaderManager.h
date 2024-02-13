#pragma once

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class ShaderManager {
public:
    static ShaderManager* get() {
        static ShaderManager* instance = new ShaderManager;
        return instance;
    }

    CCGLProgram* shader;
    CCRenderTexture* texture;
    CCSprite* sprite;

    bool isActive = false;
};