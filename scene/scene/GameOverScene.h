#pragma once

#include <Framework.h>
#include <BaseScene.h>
#include <SceneManager.h>
#include <Sprite.h>
#include <Object3d.h>
#include <thread>

#include "../../Objects/Skydome/Skydome.h"

class GameOverScene : public BaseScene
{
public:
	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

private: // メンバ変数
	// カメラ
	Camera* camera = nullptr;
	// サウンド
	SoundData soundData;

	// 2Dスプライト
	std::vector<Sprite*> sprites;
	// 3Dオブジェクト
	std::vector<Object3d*> object3ds;

	SceneManager* sceneManager_;
	std::unique_ptr<Skydome>    pSkydome_ = nullptr;      // 天球

    float theta = 0.0f;
};

