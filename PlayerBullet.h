#pragma once

#include "./Objects/.GameObject/GameObject.h"

#include <Object3d.h>
#include <memory>

class PlayerBullet : public GameObject
{
public:

	PlayerBullet() = default;
	~PlayerBullet() = default;

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

	bool IsDead() const { return isDead_; }

public: // セッター

	Vector3 SetPosition(const Vector3 _position) { return position_ = _position; }

	Vector3 SetVelocity(const Vector3 _velocity) { return velocity_ = _velocity; }

private: // メンバ変数

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;

	// 位置
	Vector3 position_{};

	// 速度
	Vector3 velocity_{};

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスグラフ
	bool isDead_ = false;
};

