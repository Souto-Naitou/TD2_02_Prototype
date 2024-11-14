#include "GamePlayScene.h"

void GamePlayScene::Initialize()
{
	// --- カメラ ---
	camera = new Camera();
	camera->SetRotate({ 0.3f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,4.0f,-10.0f });
	Object3dCommon::GetInstance()->SetDefaultCamera(camera);

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

    pBoss_ = std::make_unique<Boss>();
	pBoss_->Initialize();

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
}

void GamePlayScene::Update()
{
	//カメラの更新
	camera->Update();

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

    pBoss_->Update();

#pragma endregion 3Dオブジェクト
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

    pBoss_->Draw();

	// ↑ ↑ ↑ ↑ Draw を書き込む ↑ ↑ ↑ ↑

}
