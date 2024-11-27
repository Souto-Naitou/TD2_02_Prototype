#include "BossSong.h"

#include <ModelManager.h>
#include <cmath>
#include <iostream>

#include "CalculateMath.h"

void BossSong::Initialize()
{
	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("bossAttack/voiceShock/voiceShock.obj");

	object_ = std::make_unique<Object3d>();
	object_->Initialize("voiceShock.obj");

	// 仮置き
	object_->SetSize({ 0.2f,0.2f,0.5f });


	object_->SetPosition(position_);

	size_ = { 0.01f,0.01f,0.01f };
}

void BossSong::Finalize()
{
	isInertia_ = false;
}

void BossSong::Update()
{
	object_->Update();

	position_.z  += 1.0f;
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
	isInertia_ = true;
	object_->Draw();
}
