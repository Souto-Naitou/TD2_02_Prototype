#include "GameClearScene.h"

#include <scene/Transition/SceneTransitionManager.h>
#include <scene/Transition/TransFadeInOut.h>
#include <ImGuiDebugManager/DebugManager.h>

void GameClearScene::Initialize()
{
	// --- カメラ ---
	camera = new Camera();
	camera->SetRotate({ 0.3f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,4.0f,-10.0f });
	Object3dCommon::GetInstance()->SetDefaultCamera(camera);

	// --- スプライト ---
	std::string textureFile[] = { "test/uvChecker.png" };
	for (uint32_t i = 0; i < 1; ++i) {
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

void GameClearScene::Finalize()
{
	// 各解放処理
	delete camera;
	for (Sprite* sprite : sprites) {
		delete sprite;
	}
	Audio::GetInstance()->SoundUnload(Audio::GetInstance()->GetXAudio2(), &soundData);
}

void GameClearScene::Update()
{
	// デバッグウィンドウ
	debugManager_->DrawUI();
	//カメラの更新
	camera->Update();

	for (uint32_t i = 0; i < 1; ++i) {
		Vector2 position = { 200.0f * i, 0.0f };
		sprites[i]->SetPosition(position);

		float rotation = sprites[i]->GetRotate();
		sprites[i]->SetRotate(rotation);

		Vector2 size = { 200.0f,200.0f };
		sprites[i]->SetSize(size);

		Vector4 color = sprites[i]->GetColor();
		sprites[i]->SetColor(color);

		sprites[i]->Update();
	}

	// --- シーン移行処理 ---
	// ENTERキーを押したら
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		// 次のシーンを生成
		auto fadeInOut = std::make_unique<TransFadeInOut>();
		SceneTransitionManager::GetInstance()->ChangeScene("TITLE", std::move(fadeInOut));

	}
}

void GameClearScene::Draw()
{
	// 描画前処理(Object)
	Object3dCommon::GetInstance()->PreDraw();

	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->PreDraw();

	// ↓ ↓ ↓ ↓ Draw を書き込む ↓ ↓ ↓ ↓

	for (uint32_t i = 0; i < 1; ++i) {
		sprites[i]->Draw();
	}

	// ↑ ↑ ↑ ↑ Draw を書き込む ↑ ↑ ↑ ↑
}
