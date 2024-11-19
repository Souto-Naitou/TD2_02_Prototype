#pragma once

#include "../.GameObject/GameObject.h"
#include "../../BossNormalBullet.h"
#include "../../BossPillow.h"
#include "../../BossMoon.h"
#include "../../BaseBossState.h"
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

    // 枕攻撃
    void PillowAttack();
    // 枕攻撃発生コマンド
    std::stringstream pillowAttackPopCommands;
    // 枕攻撃発生データの読み込み
    void LoadPillowPopData();
    // 枕攻撃発生のコマンド更新
    void UpdatePillowPopCommands();


    // 月攻撃
    void MoonAttack();
    // 月攻撃発生コマンド
    std::stringstream moonAttackPopCommands;
    // 月攻撃発生データの読み込み
    void LoadMoonPopData();
    // 月攻撃発生のコマンド更新
    void UpdateMoonPopCommands();

    // 弾削除
    void DeleteBullet();

    // ステートパターン
    void ChangeState(std::unique_ptr<BaseBossState> _pState);

public: // ゲッター

    // ボスHP取得
    float GetBossHP() { return hitPoint_; }

public: // セッター

    // プレイヤー位置
    void SetPlayerPosition(Vector3 _playerPosition) { playerPosition_ = _playerPosition; }

    // ボスHP
    void SetBossHP(float _hitPoint) { hitPoint_ = _hitPoint; }

private:
    std::unique_ptr<Object3d> object_ = nullptr;
    std::unique_ptr<Easing> easing_ = nullptr;

    // HP
    const float kMaxHitPoint = 160.0f;
    float hitPoint_ = kMaxHitPoint;

    // プレーヤーの位置
    Vector3 playerPosition_{};

    // 通常弾
    std::list<BossNormalBullet*> pNormalBullets_;
    // 通常弾速
    Vector3 bltVelocity_ = { 0.0f,0.0f,-0.05f };
    // 待機中フラグ
    bool isNormalWaiting_ = true;
    // 待機タイマー
    int32_t normalWaitingTimer_ = 120;

    // 枕弾
    std::list<BossPillow*> pPillowBullets_;
    // 枕弾速
    Vector3 pillowVelocity_ = { 0.0f,0.0f,-0.05f };
    // 枕待機中フラグ
    bool isPillowWaiting_ = true;
    // 枕待機タイマー
    int32_t pillowWaitingTimer_ = 220;

    // 月弾
    std::list<BossMoon*> pMoonBullets_;
    // 月弾速
    Vector3 moonVelocity_ = { 0.0f,0.0f,-0.3f };
    // 月角度
    Vector3 moonRotate_{};
    // 月待機中フラグ
    bool isMoonWaiting_ = true;
    // 月待機タイマー
    int32_t moonWaitingTimer_ = 9;

    // ステート
    std::unique_ptr<BaseBossState> pState_ = nullptr;
};