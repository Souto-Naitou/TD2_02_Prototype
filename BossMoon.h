#pragma once

#include "Objects/.GameObject/GameObject.h"

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

public: // ゲッター

    bool IsDead() const { return isDead_; }

public: // セッター

    bool SetIsDead(const bool _isDead) { return isDead_ = _isDead; }

    Vector3 SetPosition(const Vector3 _position) { return position_ = _position; }

	Vector3 SetRotation(const Vector3 _rotation) { return rotation_ = _rotation; }

    Vector3 SetVelocity(const Vector3 _velocity) { return velocity_ = _velocity; }

private: // メンバ変数

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;

	// 速度
	Vector3 velocity_{};

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 3;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスグラフ
	bool isDead_ = false;
};

