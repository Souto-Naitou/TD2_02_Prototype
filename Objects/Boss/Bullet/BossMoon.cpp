#include "BossMoon.h"

#include <ModelManager.h>
#include <fstream>

void BossMoon::Initialize()
{
	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("plane.obj");

	object_ = std::make_unique<Object3d>();
	object_->Initialize("plane.obj");

	// 仮置き
	object_->SetSize({ 3.0f,3.0f,3.0f });


	object_->SetPosition(position_);
	object_->SetRotate(rotation_);

	collisionManager_ = CollisionManager::GetInstance();

	objectName_ = "BossMoon";

	RunSetMask();

	collider_.SetOwner(this);
	collider_.SetColliderID(objectName_);
	collider_.SetShapeData(&aabb_);
	collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
	collider_.SetShape(Shape::AABB);
	collider_.SetOnCollisionTrigger(std::bind(&BossMoon::OnCollision, this));
	collisionManager_->RegisterCollider(&collider_);
}

void BossMoon::Update()
{
	object_->Update();

	object_->SetPosition(position_);
	object_->SetRotate(rotation_);

	position_ += velocity_;

	aabb_.min = position_ - object_->GetSize();
	aabb_.max = position_ + object_->GetSize();
	collider_.SetPosition(position_);

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void BossMoon::Draw()
{
	object_->Draw();
}

void BossMoon::Finalize()
{
	collisionManager_->DeleteCollider(&collider_);
}

void BossMoon::OnCollision()
{
	isDead_ = true;
}

void BossMoon::RunSetMask()
{

	/// マスクの設定 (自分(指定されたid)は当たらない)
	collider_.SetMask(CollisionManager::GetInstance()->GetNewMask(collider_.GetColliderID(), "Boss"));

	collider_.SetMask(CollisionManager::GetInstance()->GetNewMask(collider_.GetColliderID(), "BossPillow"));

	collider_.SetMask(CollisionManager::GetInstance()->GetNewMask(collider_.GetColliderID(), "BossNormal"));
}
