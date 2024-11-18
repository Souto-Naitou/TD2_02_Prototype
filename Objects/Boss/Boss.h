#pragma once

#include "../.GameObject/GameObject.h"
#include "../../BossNormalBullet.h"
#include <Object3d.h>
#include <memory>
#include <Easing.h>

/// <summary>
/// ボス
/// </summary>
class Boss : public GameObject
{
public:
    Boss() = default;
    ~Boss() = default;

    void Initialize()   override;
    void Update()       override;
    void Draw()         override;
    void Finalize()     override;

    // 通常攻撃
    void NormalAttack();

    // 通常攻撃発生コマンド
    std::stringstream normalAttackPopCommands;

    // 通常攻撃発生データの読み込み
    void LoadNormalAttackPopData();

    // 通常攻撃発生のコマンド更新
    void UpdateNormalAttackPopCommands();

public: // セッター

    void SetPlayerPosition(Vector3 _playerPosition) { playerPosition_ = _playerPosition; }

private:
    std::unique_ptr<Object3d> object_ = nullptr;
    std::unique_ptr<Easing> easing_ = nullptr;

    // プレーヤーの位置
    Vector3 playerPosition_{};

    // 弾
    std::list<BossNormalBullet*> bullets_;

    // 待機中フラグ
    bool isWaiting = true;
    // 待機タイマー
    int32_t waitingTimer = 0;

    // 弾速
    Vector3 bltVelocity_ = { 0.0f,0.0f,-0.05f };
};