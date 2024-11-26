#pragma once

#include "./Objects/.GameObject/GameObject.h"

#include <Framework.h>
#include <Object3d.h>
#include <Easing.h>

class StanParticle
{
public: 

	void Initialize();

	void Update();

	void Draw();

public: // セッター

	void SetIsDead(bool _isDead) { isDead_ = _isDead; }

	void SetPlayerPos(Vector3 _position) { position_ = _position; }

public: // ゲッター

	bool IsDead()const { return isDead_; }

private:

	// 3Dオブジェクト
	std::unique_ptr<Object3d>   object_ = nullptr;

	Vector3 position_{};
	Vector3 size_ = { 0.2f,0.2f,0.2f };
	
	Vector3 speed_={0.0f,0.01f,0.0f};

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 2;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスグラフ
	bool isDead_ = false;
};

