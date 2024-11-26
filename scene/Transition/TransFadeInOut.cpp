#include "TransFadeInOut.h"
#include <TextureManager.h>
#include <ImGuiDebugManager/DebugManager.h>
#include <ImGuiTemplates/ImGuiTemplates.h>
#include <SceneManager.h>
#include <WinApp.h>

void TransFadeInOut::Initialize(const std::string& _sceneName)
{
    sceneName_ = _sceneName;

    screenWidth_ = WinApp::kClientWidth;
    screenHeight_ = WinApp::kClientHeight;

    easing_ = std::make_unique<Easing>("Trans_FadeInOut");

    sprite_ = std::make_unique<Sprite>();
    sprite_->Initialize("white32x32.png", {}, { 0,0,0,0 });
    sprite_->SetColor({ 0,0,0,0.5 });
    sprite_->SetSize({ screenWidth_, screenHeight_ });
    timer_.Start();
    easing_->Start();
    DebugManager::GetInstance()->SetComponent(
        "Transition", "FadeInOut", std::bind(&TransFadeInOut::DebugWindow, this));
}

void TransFadeInOut::Update()
{
    if (countPhase_ == 2)
    {
        isEnd_ = true;
        return;
    }
    if (easing_->GetIsEnd())
    {
        easing_->Reset();
        easing_->Start();
        countPhase_++;
    }
    if (!isChangedScene_ && countPhase_ == 1)
    {
        isChangedScene_ = true;
        SceneManager::GetInstance()->ChangeScene(sceneName_);
    }

    if (countPhase_ == 0)
    {
        opacity_ = easing_->Update();
    }
    else if (countPhase_ == 1)
    {
        opacity_ = 1.0f - easing_->Update();
    }

    sprite_->SetColor(Vector4(0, 0, 0, opacity_));
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
        ImGuiTemplate::VariableTableRow("Phase", countPhase_);
        ImGuiTemplate::VariableTableRow("Opacity", opacity_);
    };

    ImGuiTemplate::VariableTable("SCT_FadeInOut", pFunc);

#endif
}

TransFadeInOut::~TransFadeInOut()
{
    OutputDebugStringA("deleted TransFadeInOut\n");
}
