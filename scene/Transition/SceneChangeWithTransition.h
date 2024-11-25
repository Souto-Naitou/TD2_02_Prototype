#pragma once

#include "SCT_Base.h"
#include <memory>
#include <string>

class SceneChangeWithTransition
{
public:
    SceneChangeWithTransition() = default;
    ~SceneChangeWithTransition() {};

    void ChangeScene(const std::string& _sceneName, std::unique_ptr<SCT_Base>&& _scenePtr);
    void Update();
    void Draw();

private:
    std::unique_ptr<SCT_Base> pTransition_;
};