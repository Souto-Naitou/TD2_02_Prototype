#pragma once

#include <Framework.h>
#include <Object3d.h>

class Player
{
public:
	// 初期化
	void Initialize();

	// 終了
	void Finalize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private: // メンバ変数
	// カメラ
	Camera* camera = nullptr;

	// 3Dオブジェクト
	std::vector<Object3d*> object3ds;

	// 位置
	Vector3 position_{};

	// 速度
	Vector3 velocity_{};

};

