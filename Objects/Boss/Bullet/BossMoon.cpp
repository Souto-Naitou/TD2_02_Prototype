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


#ifdef _DEBUG
	// 大まかな判定の位置確認用
	object2_ = std::make_unique<Object3d>();
	object2_->Initialize("cube.obj");

#endif // _DEBUG
	


	collisionManager_ = CollisionManager::GetInstance();

	objectName_ = "BossMoon";

	collider_.SetOwner(this);
	collider_.SetColliderID(objectName_);
	collider_.SetShapeData(&aabb_);
	collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
	collider_.SetShape(Shape::AABB);
	collider_.SetOnCollisionTrigger(std::bind(&BossMoon::OnCollisionTrigger, this, std::placeholders::_1));
	collisionManager_->RegisterCollider(&collider_);
	collider_.SetMask(collisionManager_->GetNewMask(collider_.GetColliderID(), "Boss"));


	wholeObjectName_ = "WholeBossMoon";

	wholeCollider_.SetOwner(this);
	wholeCollider_.SetColliderID(wholeObjectName_);
	wholeCollider_.SetShapeData(&aabbWhole_);
	wholeCollider_.SetAttribute(collisionManager_->GetNewAttribute(wholeCollider_.GetColliderID()));
	wholeCollider_.SetShape(Shape::AABB);
	wholeCollider_.SetOnCollisionTrigger(std::bind(&BossMoon::WholeOnCollisionTrigger, this, std::placeholders::_1));
	collisionManager_->RegisterCollider(&wholeCollider_);
	wholeCollider_.SetMask(collisionManager_->GetNewMask(wholeCollider_.GetColliderID(), "Boss"));
}

void BossMoon::Update()
{
	object_->Update();

	object_->SetPosition(position_);
	object_->SetRotate(rotation_);

	// 0°左上想定
	if (rotation_.z >= 0.0f && rotation_.z <= 0.0f)
	{
		position_.x -= object_->GetSize().x / 2;
	}
	// 90°左下想定
	if (rotation_.z >= 1.0f && rotation_.z <= 2.0f)
	{
		position_.x -= object_->GetSize().x / 2;
		position_.y -= object_->GetSize().y;
	}
	// 180°右下想定
	if (rotation_.z >= 3.0f && rotation_.z <= 4.0f)
	{
		position_.x += object_->GetSize().x / 2;
		position_.y -= object_->GetSize().y;
	}
	// 270°右上想定
	if (rotation_.z >= 4.0f && rotation_.z <= 5.0f)
	{
		position_.x += object_->GetSize().x / 2;
	}


#ifdef _DEBUG

	// 大まかな判定の位置確認用
	object2_->Update();
	object2_->SetPosition(position_);
	object2_->SetSize(object_->GetSize() / 2);


#endif // _DEBUG

	


	aabb_.min = position_ - object_->GetSize() / 2;
	aabb_.max = position_ + object_->GetSize() / 2;
	collider_.SetPosition(position_);


	position_ = object_->GetPosition();

	aabbWhole_.min = position_ - object_->GetSize();
	aabbWhole_.max = position_ + object_->GetSize();
	aabbWhole_.min.z = position_.z - object_->GetSize().z / 2.5f;
	aabbWhole_.max.z = position_.z + object_->GetSize().z / 2.5f;
	wholeCollider_.SetPosition(position_);

	position_ += velocity_;

	//isHPRock_ = false;

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void BossMoon::Draw()
{
	object_->Draw();

#ifdef _DEBUG
	// 大まかな判定の位置確認用
	object2_->Draw();

#endif // _DEBUG
}

void BossMoon::Finalize()
{
	collisionManager_->DeleteCollider(&collider_);
	collisionManager_->DeleteCollider(&wholeCollider_);
}

void BossMoon::OnCollisionTrigger(const Collider* _other)
{
	// 一旦の処理
	if (_other->GetColliderID() == "Player")
	{
		isHit_ = true;
	}
}

void BossMoon::WholeOnCollisionTrigger(const Collider* _other)
{
	if (_other->GetColliderID() == "Player" && !isHit_)
	{
		
	}
}

