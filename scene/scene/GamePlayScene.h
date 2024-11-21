#pragma once
#include <Framework.h>
#include <BaseScene.h>
#include <Sprite.h>
#include <Object3d.h>

#include <EasingManager/EasingManager.h>
#include "../../Objects/Player/Player.h"
#include "../../Objects/Boss/Boss.h"
#include "../../Objects/Skydome/Skydome.h"

class GamePlayScene : public BaseScene
{
public:
    // 初期化
    void Initialize() override;

    // 終了
    void Finalize() override;

    // 更新処理
    void Update() override;

    // 描画処理
    void Draw() override;

private: // メンバ変数
    // カメラ
    Camera* camera = nullptr;
    EasingManager* easingManager_ = nullptr;

    // 2Dスプライト
    std::vector<Sprite*>    sprites;
    // 3Dオブジェクト
    std::vector<Object3d*>  object3ds;

    //player
    std::unique_ptr<Player>     pPlayer_     = nullptr;
    std::unique_ptr<Boss>       pBoss_      = nullptr; // ボス
    std::unique_ptr<Skydome>    pSkydome_   = nullptr; // 天球

    // サウンド
    SoundData soundDataSet;
    SoundData soundDataSet2;
};

