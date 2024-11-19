#include "Boss.h"

#include <ModelManager.h>
#include "Collision/CollisionManager/CollisionManager.h"

void Boss::Initialize()
{
    CollisionManager* collisionManager = CollisionManager::GetInstance();

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

    collider_.SetOwner(this);
    collider_.SetColliderID("Boss");
    collider_.SetShapeData(&aabb_);
    collider_.SetAttribute(collisionManager->GetNewAttribute(collider_.GetColliderID()));
    collider_.SetShape(Shape::AABB);
    collisionManager->RegisterCollider(&collider_);
}

void Boss::Update()
{
    Vector3 point1 = { -2.0f, 0.0f, 5.0f };
    Vector3 point2 = { 2.0f, 0.0f, 5.0f };

    if (easing_->GetIsEnd())
    {
        easing_->Reset();
    }

    position_.Lerp(point1, point2, easing_->Update());

    object_->SetPosition(position_);
    object_->Update();

    aabb_.min = position_ - object_->GetSize();
    aabb_.max = position_ + object_->GetSize();
    collider_.SetPosition(position_);
}

void Boss::Draw()
{
    object_->Draw();
}

void Boss::Finalize()
{
    CollisionManager::GetInstance()->DeleteCollider(&collider_);
}

void Boss::RunSetMask()
{
    /// マスクの設定 (自分(指定されたid)は当たらない)
    collider_.SetMask(CollisionManager::GetInstance()->GetNewMask(collider_.GetColliderID()));
}
