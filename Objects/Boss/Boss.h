#pragma once

#include "../.GameObject/GameObject.h"
#include "Bullet/BossNormalBullet.h"
#include "Bullet/BossPillow.h"
#include "Bullet/BossMoon.h"
#include "Bullet/BossSong.h"
#include "State/BaseBossState.h"
#include <Object3d.h>
#include <memory>
#include <Easing.h>
#include "Collision/Collider/Collider.h"
#include "Collision/CollisionManager/CollisionManager.h"
#include "Helper/Shape.h"
#include "CSVLoader.h"

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
    // リセット
    void ResetNormalAttackPopCommands();


    // 枕攻撃
    void PillowAttack();
    // 枕攻撃発生コマンド
    std::stringstream pillowAttackPopCommands;
    // 枕攻撃発生データの読み込み
    void LoadPillowPopData();
    // 枕攻撃発生のコマンド更新
    void UpdatePillowPopCommands();
    // リセット
    void ResetPillowAttackPopCommands();


    // 月攻撃
    void MoonAttack();
    // 月攻撃発生コマンド
    std::stringstream moonAttackPopCommands;
    // 月攻撃発生データの読み込み
    void LoadMoonPopData();
    // 月攻撃発生のコマンド更新
    void UpdateMoonPopCommands();
    // リセット
    void ResetMoonPopCommands();


    // 歌攻撃
    void SongAttack();
    // 歌攻撃発生コマンド
    std::stringstream songAttackPopCommands;
    // 歌攻撃発生データの読み込み
    void LoadSongPopData();
    // 歌攻撃発生のコマンド更新
    void UpdateSongPopCommands();
    // リセット
    void ResetSongPopCommands();

    // 弾削除
    void DeleteBullet();

    // ステートパターン
    void ChangeState(std::unique_ptr<BaseBossState> _pState);

private: /// 衝突判定

    void OnCollision();

public: /// ゲッター

    // ボスHP取得
    float GetBossHP() { return hp_; }

    // 通常弾取得
    const std::list<BossNormalBullet*>& GetNormalBullets() const { return pNormalBullets_; }

    // 枕取得
    const std::list<BossPillow*>& GetPillows() const { return pPillowBullets_; }

    // 歌取得
    const std::list<BossSong*>& GetSongs() const { return pSongBullets_; }

    // 月取得
    const std::list<BossMoon*>& GetMoons() const { return pMoonBullets_; }

    // スタンフラグ取得
    bool IsStan() const { return isStan_; }
    // ジャマーフラグ取得
    bool IsNarrow() const { return isNarrow_; }
    // 慣性フラグ取得
    bool IsInertia() const { return isInertia_; }

    bool IsHit() const { return isHit_; }

public: /// セッター

    // プレイヤー位置
    void SetPlayerPosition(Vector3 _playerPosition) { playerPosition_ = _playerPosition; }

    // ボスHP
    void SetBossHP(float _hitPoint) { hp_ = _hitPoint; }

    // スタン
    void SetIsStan(bool _isStan) { isStan_ = _isStan; }
    // ジャマー
    void SetIsNarrow(bool _isNarrow) { isNarrow_ = _isNarrow; }
    // 慣性
    void SetIsInertia(bool _isInertia) { isInertia_ = _isInertia; }

    void SetIsHit(bool _isHit) { isHit_ = _isHit; }

private: /// メンバ変数
    std::unique_ptr<Object3d> object_ = nullptr;
    std::unique_ptr<Easing> easing_ = nullptr;
    Collider collider_;
    AABB aabb_;
    CSVData* csvData_ = nullptr;
    std::unique_ptr<Timer> pTimer_ = nullptr;
    Vector3 destPosition_ = {};
    double kRelollTime = 0.0;

private:
    void DebugWindow() override;
    void OutputCSV();
    void UpdateRandomMovement();

    // HP
    const float kMaxHitPoint = 160.0f;

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
    bool isHit_ = false;

    // 歌弾
    std::list<BossSong*> pSongBullets_;
    // 歌待機中フラグ
    bool isSongWaiting_ = true;
    // 歌待機タイマー
    int32_t songWaitingTimer_ = 50;

    // ステート
    std::unique_ptr<BaseBossState> pState_ = nullptr;

    CollisionManager* collisionManager_ = nullptr;

    // 異常状態フラグ
    bool isStan_ = false;
    bool isNarrow_ = false;
    bool isInertia_ = false;

#ifdef _DEBUG

    uint32_t selectState_ = 0;

#endif // _DEBUG

};