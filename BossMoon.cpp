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
}

void BossMoon::Update()
{
	object_->Update();

	object_->SetPosition(position_);
	object_->SetRotate(rotation_);

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
	ModelManager::GetInstance()->Finalize();
}
