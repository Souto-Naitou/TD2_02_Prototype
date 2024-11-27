#include "TitleScene.h"

#include <scene/Transition/SceneTransitionManager.h>
#include <scene/Transition/TransFadeInOut.h>
#include <ImGuiDebugManager/DebugManager.h>

void TitleScene::Initialize()
{
	// --- カメラ ---
	camera = new Camera();
	camera->SetRotate({ 0.3f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,4.0f,-10.0f });
	Object3dCommon::GetInstance()->SetDefaultCamera(camera);

	// --- スプライト ---
	std::string textureFile[] = { "title.png" };
	for (uint32_t i = 0; i < 1; ++i) {
		Sprite* sprite = new Sprite();
		sprite->Initialize(textureFile[i], { 0,0 }, { 1,1,1,1 }, { 0,0 });

		sprites.push_back(sprite);
	}
	sceneManager_ = SceneManager::GetInstance();
    debugManager_ = DebugManager::GetInstance();

    /// 必要なモデルの読み込み
	ModelManager::GetInstance()->LoadModel("title/title.obj");
	ModelManager::GetInstance()->LoadModel("Boss/boss.obj");
	ModelManager::GetInstance()->LoadModel("test/obj/plane.obj");
	ModelManager::GetInstance()->LoadModel("test/axis.obj");
	ModelManager::GetInstance()->LoadModel("skydome/skydome.obj");
	ModelManager::GetInstance()->LoadModel("bossAttack/pillow/pillow.obj");
	ModelManager::GetInstance()->LoadModel("bossAttack/default/default.obj");
    ModelManager::GetInstance()->LoadModel("player/player.obj");
	


	for (uint32_t i = 0; i < 1; ++i) {
		Object3d* object = new Object3d();
		if (i == 0) {
			object->Initialize("title.obj");
		}
	
		Vector3 position = { 0.0f,2.0f,0.0f };
		Vector3 size = { 0.5f,0.5f,0.5f };
		object->SetPosition(position);
		object->SetSize(size);
		object3ds.push_back(object);
	}

	// 天球
	pSkydome_ = std::make_unique<Skydome>();
	pSkydome_->Initialize();

	//soundData = Audio::GetInstance()->LoadWav("BGM.wav");

	//Audio::GetInstance()->PlayWave(soundData, true, 0.3f);
}

void TitleScene::Finalize()
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

	pSkydome_->Finalize();
}

void TitleScene::Update()
{
    // デバッグウィンドウ
	debugManager_->DrawUI();
	//カメラの更新
	camera->Update();

	for (uint32_t i = 0; i < 1; ++i) {
		

		Vector2 size = { 1600.0f,900.0f };
		sprites[i]->SetSize(size);

		Vector4 color = sprites[i]->GetColor();
		sprites[i]->SetColor(color);

		sprites[i]->Update();
	}

	for (uint32_t i = 0; i < object3ds.size(); ++i) {
		Object3d* obj = object3ds[i];
		obj->Update();
	}

	// 天球の更新処理
	pSkydome_->Update();


	// --- シーン移行処理 ---
	// ENTERキーを押したら
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		
		// 次のシーンを生成
        auto fadeInOut = std::make_unique<TransFadeInOut>();
        SceneTransitionManager::GetInstance()->ChangeScene("GAMEPLAY", std::move(fadeInOut));

	}
}

void TitleScene::Draw()
{
	// 描画前処理(Object)
	Object3dCommon::GetInstance()->PreDraw();

	for (auto& obj : object3ds) {
		obj->Draw();
	}

	// 天球描画
	pSkydome_->Draw();

	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->PreDraw();

	// ↓ ↓ ↓ ↓ Draw を書き込む ↓ ↓ ↓ ↓

	for (uint32_t i = 0; i < 1; ++i) {
		sprites[i]->Draw();
	}

	// ↑ ↑ ↑ ↑ Draw を書き込む ↑ ↑ ↑ ↑
}
