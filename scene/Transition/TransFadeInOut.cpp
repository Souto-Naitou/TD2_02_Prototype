#include "TransFadeInOut.h"
#include <TextureManager.h>
#include <ImGuiDebugManager/DebugManager.h>
#include <ImGuiTemplates/ImGuiTemplates.h>


void TransFadeInOut::Initialize(const std::string& _sceneName)
{
    sceneName_ = _sceneName;
    sprite_ = std::make_unique<Sprite>();
    sprite_->Initialize("white32x32.png", {}, { 0,0,0,1 });
    sprite_->SetSize({ 1280, 720 });
    timer_.Start();
    DebugManager::GetInstance()->SetComponent(
        "Transition", "FadeInOut", std::bind(&TransFadeInOut::DebugWindow, this));
}

void TransFadeInOut::Update()
{
    if (timer_.GetNow() > 3.0)
    {
        isEnd_ = true;
    }
    sprite_->Update();
}

void TransFadeInOut::Draw()
{
    sprite_->Draw();
}

void TransFadeInOut::Finalize()
{
    DebugManager::GetInstance()->DeleteComponent("Transition", "FadeInOut");
}

void TransFadeInOut::DebugWindow()
{
#ifdef _DEBUG

    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("Scene Name", sceneName_);
        ImGuiTemplate::VariableTableRow("Timer", timer_.GetNow());
        ImGuiTemplate::VariableTableRow("TimerIsStart", timer_.GetIsStart());
    };

    ImGuiTemplate::VariableTable("SCT_FadeInOut", pFunc);

#endif
}

TransFadeInOut::~TransFadeInOut()
{
    OutputDebugStringA("deleted TransFadeInOut\n");
}
