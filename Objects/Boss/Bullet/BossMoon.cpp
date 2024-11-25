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

	collider_.SetOwner(this);
	collider_.SetColliderID(objectName_);
	collider_.SetShapeData(&aabb_);
	collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
	collider_.SetShape(Shape::AABB);
	collider_.SetOnCollisionTrigger(std::bind(&BossMoon::OnCollisionTrigger, this, std::placeholders::_1));
	collisionManager_->RegisterCollider(&collider_);
}

void BossMoon::Update()
{
	object_->Update();

	object_->SetPosition(position_);
	object_->SetRotate(rotation_);

	// 45°左上
	if (rotation_.z <= 0.8f && rotation_.z >= 0.7f)
	{
		position_.x -= object_->GetSize().x / 4;
		position_.y -= object_->GetSize().y / 4;
	}
	// 135°左下
	if (rotation_.z <= 3.0f && rotation_.z >= 2.0f)
	{
		position_.x -= object_->GetSize().x / 4;
		position_.y += object_->GetSize().y / 4;
	}
	// 225°右下
	if (rotation_.z <= 4.0f && rotation_.z >= 3.0f)
	{
		position_.x += object_->GetSize().x / 4;
		position_.y += object_->GetSize().y / 4;
	}
	// 315°右上
	if (rotation_.z <= 6.0f && rotation_.z >= 5.0f)
	{
		position_.x += object_->GetSize().x / 4;
		position_.y -= object_->GetSize().y / 4;
	}

	aabb_.min = position_ - object_->GetSize() / 2;
	aabb_.max = position_ + object_->GetSize() / 2;
	collider_.SetPosition(position_);


	position_ = object_->GetPosition();

	position_ += velocity_;

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

void BossMoon::OnCollisionTrigger(const Collider* _other)
{
}

