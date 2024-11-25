#pragma once

#include "SCT_Base.h"
#include <string>
#include <Sprite.h>


class SCT_FadeInOut : public SCT_Base
{
public:
    void Initialize(const std::string& _sceneName) override;
    void Update() override;
    void Draw() override;
    void Finalize() override;

private:
    std::string sceneName_;
    Sprite sprite_;
};