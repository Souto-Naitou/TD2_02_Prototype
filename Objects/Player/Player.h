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

    // 視野狭まる
    void Narrow();

private: // 衝突判定

	void OnCollision();

public: // ゲッター
    
    Vector3 GetPosition() { return position_; }

    bool IsStan() { return isStan_; }
    bool IsNarrow() { return isNarrow_; }

public: // セッター

    void SetCamera(Camera* _camera) { mainCamera_ = _camera; }

    void SetIsStan(bool _isStan) { isStan_ = _isStan; }
    void SetIsNarrow(bool _isNarrow) { isNarrow_ = _isNarrow; }

private: // メンバ変数

    // 3Dオブジェクト
    std::unique_ptr<Object3d>   object_                 = nullptr;

    Vector3                     moveVelocity_           = {}; // 移動速度
    float                       moveSpeed_              = 0.05f;

    std::list<PlayerBullet*>    bullets_                = {};  // 弾

    const int                   kShootCoolDownFrame_    = 15; // 発射クールタイム
    int                         countCoolDownFrame_     = 0; // 弾のクールタイム

    Vector3 cameraRotate_ = { 0.3f, 0.0f, 0.0f };
    Vector3 cameraDistance_ = { 0.0f, 2.5f, -5.5f };
    float camFollowMultiply_ = 0.1f;

    Vector2 mousePosDiff_;
    bool cursorVisible_ = true;
    bool cursorLock_ = false;


	CollisionManager* collisionManager_ = nullptr;
	Collider collider_;
	AABB aabb_;

	// 速度
	Vector3 velocity_{};

    // 状態異常
    // スタン
    bool isStan_ = false;
    const int kStanTime_ = 60 * 3;
    const int kStanCount_ = 1;
    int stanTimer_ = kStanTime_;
    
    // 視野狭まる
    bool isNarrow_ = false;
    const int kNarrowTime_ = 60 * 3;
    const int kNarrowCount_ = 1;
    int narrowTimer_ = kNarrowTime_;

private:
    /// <summary>
    /// カメラ追従処理
    /// </summary>
    void CameraFollow();

    /// <summary>
    /// デバッグ用ウィンドウ
    /// </summary>
    void DebugWindow() override;

    /// <summary>
    /// カーソル移動処理
    /// </summary>
    void CalcCursorMove();

private: /// 他クラスのやつ
    Camera* mainCamera_ = nullptr;
};

