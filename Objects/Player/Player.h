#pragma once

#include "../.GameObject/GameObject.h"
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

public: // ゲッター
    Vector3 GetPosition() { return position_; }


public: // セッター
    void SetCamera(Camera* _camera) { mainCamera_ = _camera; }


private: // メンバ変数

    // 3Dオブジェクト
    std::unique_ptr<Object3d> object_ = nullptr;

    // 速度
    Vector3 velocity_{};

    // 弾
    std::list<PlayerBullet*> bullets_;

    // 弾速
    Vector3 bltVelocity_{};

    // 弾CT
    const float kBltCoolTime = 15.0f;
    float bltCoolTime_ = 0.0f;

    Vector3 cameraRotate_ = { 0.3f, 0.0f, 0.0f };
    Vector3 cameraDistance_ = { 0.0f, 2.5f, -5.5f };
    float camFollowMultiply_ = 0.1f;
private:
    /// <summary>
    /// カメラ追従処理
    /// </summary>
    void CameraFollow();

    /// <summary>
    /// デバッグ用ウィンドウ
    /// </summary>
    void DebugWindow() override;

private: /// 他クラスのやつ
    Camera* mainCamera_ = nullptr;
};

