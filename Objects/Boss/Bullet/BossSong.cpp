#include "BossSong.h"

#include <ModelManager.h>
#include <cmath>
#include <iostream>

#include "CalculateMath.h"

void BossSong::Initialize()
{
	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("plane.obj");

	object_ = std::make_unique<Object3d>();
	object_->Initialize("plane.obj");

	// 仮置き
	object_->SetSize({ 0.2f,0.2f,0.5f });


	object_->SetPosition(position_);

	size_ = { 0.1f,0.1f,0.1f };
}

void BossSong::Finalize()
{
}

void BossSong::Update()
{
	object_->Update();

	object_->SetPosition(position_);
	object_->SetSize(scale_);

	scale_ += size_;

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void BossSong::Draw()
{

	object_->Draw();
}
