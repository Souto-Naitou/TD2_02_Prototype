#include "GamePlayScene.h"
#include "Helper/ImGuiDebugManager/DebugManager.h"

void GamePlayScene::Initialize()
{
    debugManager_ = DebugManager::GetInstance();
    collisionManager_ = CollisionManager::GetInstance();
    collisionManager_->Initialize();

    // --- カメラ ---
    camera = new Camera();
    camera->SetRotate({ 0.3f,0.0f,0.0f });
    camera->SetTranslate({ 0.0f,4.0f,-10.0f });
    Object3dCommon::GetInstance()->SetDefaultCamera(camera);

    // --- イージング ---
    easingManager_ = EasingManager::GetInstance();

    // --- スプライト ---
    std::string textureFile[] = { "test/uvChecker.png","monsterBall.png" };
    for (uint32_t i = 0; i < 1; ++i) {
        Sprite* sprite = new Sprite();
        sprite->Initialize(textureFile[i], { 0,0 }, { 1,1,1,1 }, { 0,0 });

        sprites.push_back(sprite);
    }

    // --- 3Dオブジェクト ---
    ModelManager::GetInstance()->LoadModel("test/obj/plane.obj");
    ModelManager::GetInstance()->LoadModel("test/axis.obj");

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
	soundDataSet = Audio::GetInstance()->LoadWav("fanfare.wav");
	Audio::GetInstance()->PlayWave(soundDataSet, false, 0.02f);

    soundDataSet2 = Audio::GetInstance()->LoadWav("test/xxx.wav");
    Audio::GetInstance()->PlayWave(soundDataSet2, false, 0.01f);
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
    Object3dCommon::GetInstance()->Finalize();
    ModelManager::GetInstance()->Finalize();
    Audio::GetInstance()->SoundUnload(Audio::GetInstance()->GetXAudio2(), &soundDataSet);
    Audio::GetInstance()->SoundUnload(Audio::GetInstance()->GetXAudio2(), &soundDataSet2);

	pPlayer_->Finalize();
    pBoss_->Finalize();
    pSkydome_->Finalize();
}

void GamePlayScene::Update()
{
    debugManager_->DrawUI();

    //カメラの更新
    camera->Update();

    // イージングの更新
    easingManager_->DrawUI();

#pragma region スプライト

    for (uint32_t i = 0; i < 1; ++i) {
        Vector2 position = { 200.0f * i, 0.0f };
        sprites[i]->SetPosition(position);

        float rotation = sprites[i]->GetRotate();
        sprites[i]->SetRotate(rotation);

        //Vector2 size = { 200.0f,200.0f };
        //sprites[i]->SetSize(size);

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

    // 天球の更新処理
    pSkydome_->Update();


    collisionManager_->CheckAllCollision();
}

void GamePlayScene::Draw()
{
    // 描画前処理(Sprite)
    SpriteCommon::GetInstance()->PreDraw();

    // 描画前処理(Object)
    Object3dCommon::GetInstance()->PreDraw();

    // 描画前処理(Sprite)
    SpriteCommon::GetInstance()->PreDraw();

    // ↓ ↓ ↓ ↓ Draw を書き込む ↓ ↓ ↓ ↓

    //for (uint32_t i = 0; i < 1; ++i) {
    //	sprites[i]->Draw();
    //}

    //for (auto& obj : object3ds) {
    //	obj->Draw();
    //}

    // プレーヤー描画
	pPlayer_->Draw();
    // ボス描画
    pBoss_->Draw();
    // 天球描画
    pSkydome_->Draw();

    // ↑ ↑ ↑ ↑ Draw を書き込む ↑ ↑ ↑ ↑

}
