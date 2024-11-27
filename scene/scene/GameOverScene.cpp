#include "GameOverScene.h"

#include <scene/Transition/SceneTransitionManager.h>
#include <scene/Transition/TransFadeInOut.h>
#include <ImGuiDebugManager/DebugManager.h>

void GameOverScene::Initialize()
{
	// --- カメラ ---
	camera = new Camera();
	camera->SetRotate({ 0.3f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,4.0f,-10.0f });
	Object3dCommon::GetInstance()->SetDefaultCamera(camera);

	// --- スプライト ---
	std::string textureFile[] = { "gameOver.png" ,"goTitle.png"};
	for (uint32_t i = 0; i < 2; ++i) {
		Sprite* sprite = new Sprite();
		sprite->Initialize(textureFile[i], { 0,0 }, { 1,1,1,1 }, { 0,0 });

		sprites.push_back(sprite);
	}
	sceneManager_ = SceneManager::GetInstance();
	debugManager_ = DebugManager::GetInstance();
	//ModelManager::GetInstance()->LoadModel("Boss/boss.obj");
	//ModelManager::GetInstance()->LoadModel("test/obj/plane.obj");
	//ModelManager::GetInstance()->LoadModel("plane.obj");
}

void GameOverScene::Finalize()
{
	// 各解放処理
	delete camera;
	for (Sprite* sprite : sprites) {
		delete sprite;
	}
	Audio::GetInstance()->SoundUnload(Audio::GetInstance()->GetXAudio2(), &soundData);
}

void GameOverScene::Update()
{
	// デバッグウィンドウ
	debugManager_->DrawUI();
	//カメラの更新
	camera->Update();

	for (uint32_t i = 0; i < 2; ++i) {
		
		sprites[i]->Update();

		Vector2 size = { 1600.0f,900.0f };
		sprites[i]->SetSize(size);

		Vector4 color = sprites[i]->GetColor();
		sprites[i]->SetColor(color);

		
	}

	// --- シーン移行処理 ---
	// ENTERキーを押したら
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// 次のシーンを生成
		auto fadeInOut = std::make_unique<TransFadeInOut>();
		SceneTransitionManager::GetInstance()->ChangeScene("TITLE", std::move(fadeInOut));

	}
}

void GameOverScene::Draw()
{
	// 描画前処理(Object)
	Object3dCommon::GetInstance()->PreDraw();

	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->PreDraw();

	// ↓ ↓ ↓ ↓ Draw を書き込む ↓ ↓ ↓ ↓

	for (uint32_t i = 0; i < 2; ++i) {
		sprites[i]->Draw();
	}

	// ↑ ↑ ↑ ↑ Draw を書き込む ↑ ↑ ↑ ↑
}
