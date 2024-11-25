#include "SceneChangeWithTransition.h"

void SceneChangeWithTransition::ChangeScene(const std::string& _sceneName, std::unique_ptr<SCT_Base>&& _scenePtr)
{
    pTransition_->Finalize();
    pTransition_ = std::move(_scenePtr);
    pTransition_->Initialize(_sceneName);
}

void SceneChangeWithTransition::Update()
{
    if (pTransition_ == nullptr) return;
    pTransition_->Update();
}

void SceneChangeWithTransition::Draw()
{
    if (pTransition_ == nullptr) return;
    pTransition_->Draw();
}