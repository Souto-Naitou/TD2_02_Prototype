#pragma once

#include "../.GameObject/GameObject.h"
#include "Collision/Collider/Collider.h"
#include "Bullet/PlayerBullet.h"

#include <Framework.h>
#include <Object3d.h>

class Player : public GameObject
{
public:

	Player() = default;
	~Player() = default;

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

	// 攻撃
	void Attack();

private: // 衝突判定

	void OnCollision();

public: // ゲッター

	Vector3 GetPosition() { return position_; }

private: // メンバ変数

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;

	CollisionManager* collisionManager_ = nullptr;
	Collider collider_;
	AABB aabb_;

	// 速度
	Vector3 velocity_{};

	// 弾
	std::list<PlayerBullet*> bullets_;

	// 弾速
	Vector3 bltVelocity_{};

	// 弾CT
	const float kBltCoolTime = 15.0f;
	float bltCoolTime_ = 0.0f;
};

