#include "BossPillow.h"

#include <ModelManager.h>
#include <cmath>
#include <iostream>

#include "CalculateMath.h"

void BossPillow::Initialize()
{
	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("cube.obj");

	object_ = std::make_unique<Object3d>();
	object_->Initialize("cube.obj");

	// 仮置き
	object_->SetSize({ 0.2f,0.2f,0.2f });


	object_->SetPosition(position_);
}

void BossPillow::Finalize()
{
	ModelManager::GetInstance()->Finalize();
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
	//velocity_ = 0.1f * Slerp(velocity_, toPlayer, 0.05f);
	velocity_.Lerp(velocity_, toPlayer, 0.05f);

	position_ += velocity_ * 0.1f;
	rotation_.y += 1.0f;

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void BossPillow::Draw()
{
	object_->Draw();
}
