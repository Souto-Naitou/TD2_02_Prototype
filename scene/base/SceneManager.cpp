#include "SceneManager.h"
#include <cassert>
#include <ModelManager.h>
#include <Input.h>
#include <ImGuiDebugManager/DebugManager.h>

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance()
{
    if (instance == nullptr) {
        instance = new SceneManager;
    }
    return instance;
}

void SceneManager::Finalize()
{
    scene_->Finalize();
    delete scene_;

    delete instance;
    instance = nullptr;
}

void SceneManager::Update()
{
    if (!sceneTransitionManager_) sceneTransitionManager_ = SceneTransitionManager::GetInstance();
    if (!easingManager_) easingManager_ = EasingManager::GetInstance();

    easingManager_->DrawUI();
    if (Input::GetInstance()->TriggerKey(DIK_F3))
    {
        DebugManager::GetInstance()->SetDisplay(!DebugManager::GetInstance()->GetDisplay());
        easingManager_->SetDisplay(!easingManager_->GetDisplay());
    }

    // --- シーン切り替え機構 ---
    if (nextScene_) {
        // 旧シーン終了
        if (scene_) {
            scene_->Finalize();
            delete scene_;
        }

        // シーン切り替え
        scene_ = nextScene_;
        nextScene_ = nullptr;

        // シーンマネージャをセット
        scene_->SetSceneManager(this);

        // 次のシーンを初期化
        scene_->Initialize();
    }

    // --- 実行中のシーンを更新 ---
    scene_->Update();
    sceneTransitionManager_->Update();
    ModelManager::GetInstance()->UploadModel();
}

void SceneManager::Draw()
{
    // --- 実行中のシーンを描画 ---
    scene_->Draw();
    sceneTransitionManager_->Draw();
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
    assert(sceneFactory_);
    assert(nextScene_ == nullptr);

    // 次のシーンを生成
    nextScene_ = sceneFactory_->CreateScene(sceneName);
}
