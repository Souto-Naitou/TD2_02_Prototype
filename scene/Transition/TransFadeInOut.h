#pragma once

#include "scene/Transition/TransBase.h"
#include <string>
#include <Sprite.h>
#include <Easing.h>


class TransFadeInOut : public TransBase
{
public:
    ~TransFadeInOut();
    void Initialize(const std::string& _sceneName) override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
    void DebugWindow() override;

private:

    uint32_t screenWidth_ = 1280;
    uint32_t screenHeight_ = 720;

    float opacity_ = 0.0f;

    std::unique_ptr<Easing> easing_;
    uint32_t countPhase_ = 0;

    std::string sceneName_;
    std::unique_ptr<Sprite> sprite_;
};