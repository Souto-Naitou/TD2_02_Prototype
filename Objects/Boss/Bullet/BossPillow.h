#pragma once

#include "../../.GameObject/GameObject.h"
#include "Collision/Collider/Collider.h"
#include "Collision/CollisionManager/CollisionManager.h"
#include <Object3d.h>
#include <memory>



class BossPillow : public GameObject
{
public:


	BossPillow() = default;
	~BossPillow() = default;

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

private: // 衝突判定

	void OnCollision();

public: // ゲッター

	bool IsDead() const { return isDead_; }

	bool IsNarrow() const { return isNarrow_; }

public: // セッター

	void SetPlayerPosition(Vector3 _playerPosition) { playerPosition_ = _playerPosition; }

	bool SetIsDead(const bool _isDead) { return isDead_ = _isDead; }

	Vector3 SetPosition(const Vector3 _position) { return position_ = _position; }

	Vector3 SetVelocity(const Vector3 _velocity) { return velocity_ = _velocity; }

private: // メンバ変数

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;

	// プレイヤーの位置
	Vector3 playerPosition_{};

	// 速度
	Vector3 velocity_{};

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 2;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスグラフ
	bool isDead_ = false;

	CollisionManager* collisionManager_ = nullptr;
	Collider collider_;
	AABB aabb_;

	// 状態異常付与フラグ
	bool isNarrow_ = false;

};

