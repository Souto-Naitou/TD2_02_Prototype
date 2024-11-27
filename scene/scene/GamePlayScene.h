#pragma once
#include <Framework.h>
#include <BaseScene.h>
#include <Sprite.h>
#include <Object3d.h>

#include <Collision/CollisionManager/CollisionManager.h>
#include "../../Objects/Player/Player.h"
#include "../../Objects/Boss/Boss.h"
#include "../../Objects/Skydome/Skydome.h"

class GamePlayScene : public BaseScene
{
public:
    // 初期化
    void Initialize()   override;

    // 終了
    void Finalize()     override;

    // 更新処理
    void Update()       override;

    // 描画処理
    void Draw()         override;


private: /// メンバ変数
    Camera*                     camera              = nullptr;      // カメラ
    CollisionManager*           collisionManager_   = nullptr;      // 衝突判定マネージャ
    SceneManager*               sceneManager_       = nullptr;      // シーンマネージャ

    // 2Dスプライト
    std::vector<Sprite*>        sprites             = {};           // 2Dスプライト
    // 3Dオブジェクト
    std::vector<Object3d*>      object3ds           = {};           // 3Dオブジェクト

    //player
    std::unique_ptr<Player>     pPlayer_            = nullptr;
    std::unique_ptr<Boss>       pBoss_              = nullptr;      // ボス
    std::unique_ptr<Skydome>    pSkydome_           = nullptr;      // 天球
};

