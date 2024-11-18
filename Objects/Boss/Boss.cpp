#include "Boss.h"

#include <ModelManager.h>

void Boss::Initialize()
{
    /// モデルの読み込みと登録
    ModelManager::GetInstance()->LoadModel("cube.obj");

    /// Transformの初期化
    scale_ = { 0.5f, 0.5f, 0.5f };
    position_ = { 0.0f, 0.0f, 10.0f };
    rotation_ = { 0.0f, 0.0f, 0.0f };

    object_ = std::make_unique<Object3d>();

    object_->Initialize("cube.obj");
    object_->SetSize(scale_);
    object_->SetRotate(rotation_);
    object_->SetPosition(position_);

    easing_ = std::make_unique<Easing>("TEST");
    easing_->Initialize();
}

void Boss::Update()
{
    //デスフラグの立った弾を削除
    bullets_.remove_if([](BossNormalBullet* bullet) {
        if (bullet->IsDead()) {
            delete bullet;
            //bullet->Finalize();
            return true;
        }
        return false;
        });

    Vector3 point1 = { -2.0f, 0.0f, 5.0f };
    Vector3 point2 = { 2.0f, 0.0f, 5.0f };

    if (easing_->GetIsEnd())
    {
        easing_->Reset();
    }

    position_.Lerp(point1, point2, easing_->Update());

    object_->SetPosition(position_);
    object_->Update();

    NormalAttack();

    // 弾更新
    for (auto& bullet : bullets_) {
        bullet->Update();
    }
}

void Boss::Draw()
{
    object_->Draw();

    // 弾描画
    for (auto& bullet : bullets_) {
        bullet->Draw();
    }
}

void Boss::Finalize()
{
    for (auto& bullet : bullets_) {
        bullet->SetIsDead(true);
        bullet->Finalize();
        //delete bullet;
    }

    bullets_.remove_if([](BossNormalBullet* bullet) {
        if (bullet->IsDead()) {
            delete bullet;
            //bullet->Finalize();
            return true;
        }
        return false;
        });
}

void Boss::NormalAttack()
{
    if (bltCoolTime_ <= 0)
    {
        // 弾を生成し、初期化
        BossNormalBullet* newBullet = new BossNormalBullet();

        newBullet->SetPosition(position_);
        newBullet->SetPlayerPosition(playerPosition_);
        newBullet->Initialize();
        newBullet->SetVelocity(bltVelocity_);

        // 弾を登録する
        bullets_.push_back(newBullet);

        bltCoolTime_ = kBltCoolTime;
    }

    bltCoolTime_--;
}
