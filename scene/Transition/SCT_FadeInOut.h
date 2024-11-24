#pragma once

#include "SCT_Base.h"
#include <string>

class SCT_FadeInOut : public SCT_Base
{
public:
    void Initialize(const std::string& sceneName) override;
    void Update() override;
    void Draw() override;
    void Finalize() override;

private:

};