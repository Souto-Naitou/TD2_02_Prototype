#pragma once

#include "scene/Transition/TransBase.h"
#include <string>
#include <Sprite.h>


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

    std::string sceneName_;
    std::unique_ptr<Sprite> sprite_;
};