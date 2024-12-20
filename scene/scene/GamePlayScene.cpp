#include "GamePlayScene.h"

#include <scene/Transition/SceneTransitionManager.h>
#include <scene/Transition/TransFadeInOut.h>

#include "Helper/ImGuiDebugManager/DebugManager.h"

void GamePlayScene::Initialize()
{
    //sceneManager_ = SceneManager::GetInstance();
    debugManager_ = DebugManager::GetInstance();
    collisionManager_ = CollisionManager::GetInstance();
    collisionManager_->Initialize();

    // --- カメラ ---
    camera = new Camera();
    camera->SetRotate({ 0.3f,0.0f,0.0f });
    camera->SetTranslate({ 0.0f,4.0f,-10.0f });
    Object3dCommon::GetInstance()->SetDefaultCamera(camera);


    // --- スプライト ---
    std::string textureFile[] = { "gameUI.png"};
    for (uint32_t i = 0; i < 1; ++i) {
        Sprite* sprite = new Sprite();
        sprite->Initialize(textureFile[i], { 0,0 }, { 1,1,1,1 }, { 0,0 });

        sprites.push_back(sprite);
    }

    // --- 3Dオブジェクト ---

    for (uint32_t i = 0; i < 2; ++i) {
        Object3d* object = new Object3d();
        if (i == 0) {
            object->Initialize("plane.obj");
        }
        if (i == 1) {
            object->Initialize("axis.obj");
        }

        Vector3 position;
        position.x = i * 2.0f;
        object->SetPosition(position);

        object3ds.push_back(object);
    }

    // --- 自作クラス ---

    // ボス
    pBoss_ = std::make_unique<Boss>();
    pBoss_->Initialize();

    // プレイヤー
    pPlayer_ = std::make_unique<Player>();
    pPlayer_->Initialize();
    pPlayer_->SetCamera(camera);

    // 天球
    pSkydome_ = std::make_unique<Skydome>();
    pSkydome_->Initialize();

    // --- オーディオ ---
    //soundData = Audio::GetInstance()->LoadWav("BGM.wav");

   // Audio::GetInstance()->PlayWave(soundData, true, 0.3f);
}

void GamePlayScene::Finalize()
{
    // 各解放処理
    delete camera;
    for (auto& obj : object3ds) {
        delete obj;
    }
    for (Sprite* sprite : sprites) {
        delete sprite;
    }
    Audio::GetInstance()->SoundUnload(Audio::GetInstance()->GetXAudio2(), &soundData);
    

    pPlayer_->Finalize();
    pBoss_->Finalize();
    pSkydome_->Finalize();
}

void GamePlayScene::Update()
{
    debugManager_->DrawUI();

    //カメラの更新
    camera->Update();

#pragma region スプライト

    for (uint32_t i = 0; i < 1; ++i) {

        Vector2 size = { 1600.0f,900.0f };
        sprites[i]->SetSize(size);

        Vector4 color = sprites[i]->GetColor();
        sprites[i]->SetColor(color);

        sprites[i]->Update();
    }

#pragma endregion スプライト

#pragma region 3Dオブジェクト


    for (uint32_t i = 0; i < object3ds.size(); ++i) {
        Object3d* obj = object3ds[i];
        obj->Update();

        Vector3 rotate = obj->GetRotate();
        if (i == 0) {
            rotate.x += 0.01f;
        }
        else if (i == 1) {
            rotate.y += 0.01f;
        }

        obj->SetRotate(rotate);
    }



#pragma endregion 3Dオブジェクト

    // プレーヤーの更新処理
    pPlayer_->Update();

    if (pPlayer_->IsStan())
    {
        pBoss_->SetIsStan(false);
    }
    if (pPlayer_->IsNarrow())
    {
        pBoss_->SetIsNarrow(false);
    }
    if (pPlayer_->IsInertia())
    {
        pBoss_->SetIsInertia(false);
    }

    // ボスにプレイヤーの位置をセット
    pBoss_->SetPlayerPosition(pPlayer_->GetPosition());

    // ボスの更新処理
    pBoss_->Update();

    if (pBoss_->IsStan())
    {
        pPlayer_->SetIsStan(true);
    }
    if (pBoss_->IsNarrow())
    {
        pPlayer_->SetIsNarrow(true);
    }
    if (pBoss_->IsInertia())
    {
        pPlayer_->SetIsInertia(true);
    }

    pPlayer_->SetIsHit(pBoss_->IsHit());

    // 天球の更新処理
    pSkydome_->Update();


    collisionManager_->CheckAllCollision();


    // --- シーン移行処理 ---
    // Bossが死んだ瞬間
    if (pBoss_->IsBossDeadMoment()) {
        // 次のシーンを生成
        auto fadeInOut = std::make_unique<TransFadeInOut>();
        SceneTransitionManager::GetInstance()->ChangeScene("GAMECLEAR", std::move(fadeInOut));

    }

    // Playerが死んだ瞬間
    if (pPlayer_->IsDeadMoment())
    {
        // 次のシーンを生成
        auto fadeInOut = std::make_unique<TransFadeInOut>();
        SceneTransitionManager::GetInstance()->ChangeScene("GAMEOVER", std::move(fadeInOut));

    }

#ifdef _DEBUG
    if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
        // 次のシーンを生成
        auto fadeInOut = std::make_unique<TransFadeInOut>();
        SceneTransitionManager::GetInstance()->ChangeScene("GAMECLEAR", std::move(fadeInOut));

    }
#endif // _DEBUG


    pPlayer_->SetIsDeadMoment(false);
    pBoss_->SetIsBossDeadMoment(false);
}

void GamePlayScene::Draw()
{
    // 描画前処理(Sprite)
    SpriteCommon::GetInstance()->PreDraw();

    // 描画前処理(Object)
    Object3dCommon::GetInstance()->PreDraw();

    // 天球描画
    pSkydome_->Draw();
    // プレーヤー描画
    pPlayer_->Draw();
    // ボス描画
    pBoss_->Draw();

    // 描画前処理(Sprite)
    SpriteCommon::GetInstance()->PreDraw();
   
    // ↓ ↓ ↓ ↓ Draw を書き込む ↓ ↓ ↓ ↓

    pPlayer_->Draw2d();
    pBoss_->Draw2D();

    for (uint32_t i = 0; i < 1; ++i) {
    	sprites[i]->Draw();
    }

    //for (auto& obj : object3ds) {
    //	obj->Draw();
    //}

    // ↑ ↑ ↑ ↑ Draw を書き込む ↑ ↑ ↑ ↑

}
