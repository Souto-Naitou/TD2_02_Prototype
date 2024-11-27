#pragma once

#include "../../.GameObject/GameObject.h"
#include "Collision/Collider/Collider.h"
#include "Collision/CollisionManager/CollisionManager.h"

#include <Object3d.h>
#include <memory>
#include <Easing.h>

class BossMoon : public GameObject
{
public:

    BossMoon() = default;
    ~BossMoon() = default;

    void Initialize()   override;
    void Update()       override;
    void Draw()         override;
    void Finalize()     override;

private: // 衝突判定

	void OnCollisionTrigger(const Collider* _other);

	void WholeOnCollisionTrigger(const Collider* _other);
	
public: // ゲッター

    bool IsDead() const { return isDead_; }

	bool IsHit() const { return isHit_; }

public: // セッター

    bool SetIsDead(const bool _isDead) { return isDead_ = _isDead; }

	void SetPlayerPosition(Vector3 _playerPosition) { playerPosition_ = _playerPosition; }

    Vector3 SetPosition(const Vector3 _position) { return position_ = _position; }

	Vector3 SetRotation(const Vector3 _rotation) { return rotation_ = _rotation; }

    Vector3 SetVelocity(const Vector3 _velocity) { return velocity_ = _velocity; }

private: // メンバ変数

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;

#ifdef _DEBUG
	// 大まかな判定の位置確認用
	std::unique_ptr<Object3d> object2_ = nullptr;
#endif // _DEBUG

	// プレイヤーの位置
	Vector3 playerPosition_{};

	// 速度
	Vector3 velocity_{};

	// プレーヤーに向けて
	Vector3 toPlayer_{};

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 3;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスグラフ
	bool isDead_ = false;

	CollisionManager* collisionManager_ = nullptr;
	Collider collider_;
	AABB aabb_;
	std::string wholeObjectName_;
	Collider wholeCollider_;
	AABB aabbWhole_;

	bool isHit_ = false;
};

