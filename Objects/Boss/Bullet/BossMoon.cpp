#include "BossMoon.h"

#include <ModelManager.h>
#include <fstream>

void BossMoon::Initialize()
{
	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("bossAttack/moon/moon.obj");

	object_ = std::make_unique<Object3d>();
	object_->Initialize("moon.obj");

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

	// 敵弾から自キャラへのベクトルを計算
	toPlayer_ = playerPosition_ - position_;

	
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


	// ベクトルを正規化する
	toPlayer_ = Normalize(toPlayer_);
	velocity_ = Normalize(velocity_);
	// 球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
	velocity_.Lerp(velocity_, toPlayer_, 0.05f);

	// Y軸周り角度(θy)
	rotation_.y = std::atan2(velocity_.x, velocity_.z);
	double velocityXZ = sqrt(pow(velocity_.x, 2) + pow(velocity_.z, 2));

	// X軸周り角度(θx)
	rotation_.x = (float)std::atan2(-velocity_.y, velocityXZ);

	position_ += velocity_ * 0.1f;

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

