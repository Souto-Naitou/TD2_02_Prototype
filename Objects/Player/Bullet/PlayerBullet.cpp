#include "PlayerBullet.h"

#include <Object3d.h>

#include <ModelManager.h>

void PlayerBullet::Initialize()
{
   collisionManager_ = CollisionManager::GetInstance();

	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("bullet/bullet.obj");

	object_ = std::make_unique<Object3d>();
	object_->Initialize("bullet.obj");


	// 仮置き
	object_->SetSize({ 0.2f,0.2f,0.2f });

	object_->SetPosition(position_);


    // --- コリジョン ---
	objectName_ = "PlayerBullet";
    collider_.SetOwner(this);
    collider_.SetColliderID(objectName_);
    collider_.SetShapeData(&aabb_);
    collider_.SetShape(Shape::AABB);
    collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
	collider_.SetOnCollisionTrigger(std::bind(&PlayerBullet::OnCollision, this));
	collisionManager_->RegisterCollider(&collider_);
}

void PlayerBullet::Finalize()
{
	// 各解放処理
	
	collisionManager_->DeleteCollider(&collider_);
}

void PlayerBullet::Update()
{
	object_->Update();

	object_->SetPosition(position_);

	position_ += velocity_;

	aabb_.min = position_ - object_->GetSize();
    aabb_.max = position_ + object_->GetSize();
    collider_.SetPosition(position_);

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw()
{
	object_->Draw();
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

void PlayerBullet::RunSetMask()
{
    collider_.SetMask(collisionManager_->GetNewMask(collider_.GetColliderID(), "Player"));
}
