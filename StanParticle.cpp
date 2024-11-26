#include "StanParticle.h"



// 指定した範囲のfloat型乱数を生成する関数
float GetRandomFloat(float min, float max) {
	float random = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
	return min + random * (max - min);
}

void StanParticle::Initialize()
{
	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("bossAttack/default/default.obj");

	object_ = std::make_unique<Object3d>();
	object_->Initialize("default.obj");

	object_->SetPosition(position_);
	object_->SetSize(size_);
}

void StanParticle::Update()
{
	object_->Update();

	object_->SetPosition(position_);
	object_->SetSize(size_);

	position_ += speed_;

	size_ -= {0.0016f, 0.0016f, 0.0016f};

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void StanParticle::Draw()
{
	if (!isDead_)
	{
		object_->Draw();
	}
}
