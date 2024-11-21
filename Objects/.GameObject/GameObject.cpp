#include "GameObject.h"
#include <Helper/ImGuiDebugManager/DebugManager.h>
#include <functional>
#include <sstream>


GameObject::GameObject()
{
    objectName_ = "unnamed-object";
    scale_ = { 1.0f, 1.0f, 1.0f };
    rotation_ = { 0.0f, 0.0f, 0.0f };
    position_ = { 0.0f, 0.0f, 0.0f };
    hp_ = 100.0f;
}

void GameObject::RegisterDebugWindow()
{
    std::stringstream ss;
    ss << std::hex << this;
    DebugManager::GetInstance()->SetComponent(objectName_, ss.str(), std::bind(&GameObject::DebugWindow, this));
}

void GameObject::UnregisterDebugWindow()
{
    std::stringstream ss;
    ss << std::hex << this;
    DebugManager::GetInstance()->DeleteComponent(objectName_.c_str(), ss.str().c_str());
}
