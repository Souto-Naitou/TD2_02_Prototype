#include "BossPillow.h"

#include <ModelManager.h>
#include <cmath>
#include <iostream>

#include "CalculateMath.h"

void BossPillow::Initialize()
{
	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("bossAttack/pillow/pillow.obj");

	object_ = std::make_unique<Object3d>();
	object_->Initialize("pillow.obj");

	// 仮置き
	object_->SetSize({ 0.2f,0.2f,0.2f });

	object_->SetPosition(position_);


	collisionManager_ = CollisionManager::GetInstance();

	objectName_ = "BossPillow";

	collider_.SetOwner(this);
	collider_.SetColliderID(objectName_);
	collider_.SetShapeData(&aabb_);
	collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
	collider_.SetShape(Shape::AABB);
	collider_.SetOnCollisionTrigger(std::bind(&BossPillow::OnCollisionTrigger, this, std::placeholders::_1));
	collisionManager_->RegisterCollider(&collider_);
}

void BossPillow::Finalize()
{
	isNarrow_ = false;
	collisionManager_ ->DeleteCollider(&collider_);
}

void BossPillow::Update()
{
	object_->Update();

	object_->SetPosition(position_);
	object_->SetRotate(rotation_);


	// 敵弾から自キャラへのベクトルを計算
	Vector3 toPlayer = playerPosition_ - position_;

	// ベクトルを正規化する
	toPlayer = Normalize(toPlayer);
	velocity_ = Normalize(velocity_);
	// 球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
	velocity_.Lerp(velocity_, toPlayer, 0.05f);

	position_ += velocity_ * 0.1f;
	rotation_.y += kRotationSpeed_;

	aabb_.min = position_ - object_->GetSize();
	aabb_.max = position_ + object_->GetSize();
	collider_.SetPosition(position_);

	

	//時間経過でデス
	if (--deathTimer_ <= 0) 
	{
		isDead_ = true;
	}
}

void BossPillow::Draw()
{
	object_->Draw();
}

void BossPillow::OnCollisionTrigger(const Collider* _other)
{
	if (_other->GetColliderID() == "Player")
	{
		isNarrow_ = true;
	}
	isDead_ = true;
}
