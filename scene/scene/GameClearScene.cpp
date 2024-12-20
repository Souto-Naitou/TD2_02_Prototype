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
	std::string textureFile[] = { "gameClear.png" ,"goTitle.png" };
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

	// 天球
	pSkydome_ = std::make_unique<Skydome>();
	pSkydome_->Initialize();
}

void GameClearScene::Finalize()
{
	// 各解放処理
	delete camera;
	for (Sprite* sprite : sprites) {
		delete sprite;
	}
	Audio::GetInstance()->SoundUnload(Audio::GetInstance()->GetXAudio2(), &soundData);

	pSkydome_->Finalize();
}

void GameClearScene::Update()
{
	// デバッグウィンドウ
	debugManager_->DrawUI();
	//カメラの更新
	camera->Update();

	for (uint32_t i = 0; i < 2; ++i) {

		Vector2 size = { 1600.0f,900.0f };
		sprites[i]->SetSize(size);

		Vector4 color = sprites[i]->GetColor();
		sprites[i]->SetColor(color);

		sprites[i]->Update();
	}

	// 天球の更新処理
	pSkydome_->Update();


	// --- シーン移行処理 ---
	// ENTERキーを押したら
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// 次のシーンを生成
		auto fadeInOut = std::make_unique<TransFadeInOut>();
		SceneTransitionManager::GetInstance()->ChangeScene("TITLE", std::move(fadeInOut));

	}
}

void GameClearScene::Draw()
{
	// 描画前処理(Object)
	Object3dCommon::GetInstance()->PreDraw();

	// 天球描画
	pSkydome_->Draw();

	// 描画前処理(Sprite)
	SpriteCommon::GetInstance()->PreDraw();

	// ↓ ↓ ↓ ↓ Draw を書き込む ↓ ↓ ↓ ↓

	for (uint32_t i = 0; i < 2; ++i) {
		sprites[i]->Draw();
	}

	// ↑ ↑ ↑ ↑ Draw を書き込む ↑ ↑ ↑ ↑
}
