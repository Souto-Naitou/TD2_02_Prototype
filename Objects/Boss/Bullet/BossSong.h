#pragma once

#include "../../.GameObject/GameObject.h"
#include <Object3d.h>
#include <memory>

class BossSong : public GameObject
{
public:


	BossSong() = default;
	~BossSong() = default;

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;


public: // ゲッター

	bool IsDead() const { return isDead_; }

	// 状態異常,慣性
	bool IsInertia() const { return isInertia_; }

public: // セッター

	void SetPlayerPosition(Vector3 _playerPosition) { playerPosition_ = _playerPosition; }

	bool SetIsDead(const bool _isDead) { return isDead_ = _isDead; }

	Vector3 SetPosition(const Vector3 _position) { return position_ = _position; }

private: // メンバ変数

	// 3Dオブジェクト
	std::unique_ptr<Object3d> object_ = nullptr;

	// プレイヤーの位置
	Vector3 playerPosition_{};

	// サイズ
	Vector3 size_{};

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 2;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスグラフ
	bool isDead_ = false;

	// 状態異常付与フラグ
	bool isInertia_ = false;
};

