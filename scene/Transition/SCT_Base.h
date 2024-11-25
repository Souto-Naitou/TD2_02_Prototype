#pragma once
#include <string>
#include <Timer/Timer.h>


class SCT_Base
{
public:
    virtual void Initialize(const std::string& sceneName) = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Finalize() = 0;

protected:
    Timer timer_;
};