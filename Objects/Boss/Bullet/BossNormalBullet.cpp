#include "BossNormalBullet.h"

#include <ModelManager.h>
#include <cmath>
#include <iostream>

#include "CalculateMath.h"

void BossNormalBullet::Initialize()
{
	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("cube.obj");

	object_ = std::make_unique<Object3d>();
	object_->Initialize("cube.obj");

	// 仮置き
	object_->SetSize({ 0.2f,0.2f,0.5f });


	object_->SetPosition(position_);


	collisionManager_ = CollisionManager::GetInstance();

	objectName_ = "BossNormal";

	RunSetMask();

	collider_.SetOwner(this);
	collider_.SetColliderID(objectName_);
	collider_.SetShapeData(&aabb_);
	collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
	collider_.SetShape(Shape::AABB);
	collider_.SetOnCollisionTrigger(std::bind(&BossNormalBullet::OnCollision, this));
	collisionManager_->RegisterCollider(&collider_);
}

void BossNormalBullet::Finalize()
{
	// 各解放処理
	//isDead_ = true;
	//object_.reset();

	collisionManager_->DeleteCollider(&collider_);
}

void BossNormalBullet::Update()
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
	//velocity_ = 0.1f * Slerp(velocity_, toPlayer, 0.05f);
	velocity_.Lerp(velocity_, toPlayer, 0.05f);

	// 進行方向に見た目の回転を合わせる(ex1)
	// Y軸周り角度(θy)
	rotation_.y = std::atan2(velocity_.x, velocity_.z);
	double velocityXZ = sqrt(pow(velocity_.x, 2) + pow(velocity_.z, 2));

	// X軸周り角度(θx)
	rotation_.x = (float)std::atan2(-velocity_.y, velocityXZ);

	position_ += velocity_ * 0.1f;

	aabb_.min = position_ - object_->GetSize();
	aabb_.max = position_ + object_->GetSize();
	collider_.SetPosition(position_);

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void BossNormalBullet::Draw()
{
	object_->Draw();
}

void BossNormalBullet::RunSetMask()
{
	/// マスクの設定 (自分(指定されたid)は当たらない)
	collider_.SetMask(CollisionManager::GetInstance()->GetNewMask(collider_.GetColliderID(), "Boss"));

	collider_.SetMask(CollisionManager::GetInstance()->GetNewMask(collider_.GetColliderID(), "BossPillow"));

	collider_.SetMask(CollisionManager::GetInstance()->GetNewMask(collider_.GetColliderID(), "BossMoon"));
}

void BossNormalBullet::OnCollision()
{
	isDead_ = true;
}
