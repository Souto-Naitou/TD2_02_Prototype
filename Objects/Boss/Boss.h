#pragma once

#include "../.GameObject/GameObject.h"
#include "../../BossNormalBullet.h"
#include <Object3d.h>
#include <memory>
#include <Easing.h>
#include "Collision/Collider/Collider.h"
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

    void NormalAttack();

public: // セッター

    void SetPlayerPosition(Vector3 _playerPosition) { playerPosition_ = _playerPosition; }

    void RunSetMask();

private:
    std::unique_ptr<Object3d> object_ = nullptr;
    std::unique_ptr<Easing> easing_ = nullptr;

    // プレーヤーの位置
    Vector3 playerPosition_{};

    // 弾
    std::list<BossNormalBullet*> bullets_;

    // 弾CT
    const float kBltCoolTime = 120.0f;
    float bltCoolTime_ = 0.0f;
    // 弾速
    Vector3 bltVelocity_ = { 0.0f,0.0f,-0.05f };
    Collider collider_;
    AABB aabb_;
    CSVData* csvData_ = nullptr;

private:
    void DebugWindow() override;
    void OutputCSV();
};