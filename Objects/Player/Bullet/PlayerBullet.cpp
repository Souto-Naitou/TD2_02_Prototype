#include "PlayerBullet.h"

#include <Object3d.h>
#include <Collision/CollisionManager/CollisionManager.h>

#include <ModelManager.h>

void PlayerBullet::Initialize()
{
    CollisionManager* collisionManager = CollisionManager::GetInstance();

	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("cube.obj");

	object_ = std::make_unique<Object3d>();
	object_->Initialize("cube.obj");


	// 仮置き
	object_->SetSize({ 0.2f,0.2f,0.2f });

	object_->SetPosition(position_);


    // --- コリジョン ---
    collider_.SetOwner(this);
    collider_.SetColliderID("PlayerBullet");
    collider_.SetShapeData(&aabb_);
    collider_.SetShape(Shape::AABB);
    collider_.SetAttribute(collisionManager->GetNewAttribute(collider_.GetColliderID()));
	collisionManager->RegisterCollider(&collider_);
}

void PlayerBullet::Finalize()
{
	// 各解放処理
	//isDead_ = true;
	//object_.reset();

	CollisionManager::GetInstance()->DeleteCollider(&collider_);
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

void PlayerBullet::RunSetMask()
{
    CollisionManager* collisionManager = CollisionManager::GetInstance();
    collider_.SetMask(collisionManager->GetNewMask(collider_.GetColliderID(), "Player"));
}
