#include "StanEmitter.h"
#include "random"

void StanEmitter::Initialize()
{
	unsigned int currenttime = (unsigned int)time(nullptr);
	srand(currenttime);
}

void StanEmitter::Finalize()
{
	for (auto& particle : particles_) {
		particle->SetIsDead(true);;
	}

	particles_.remove_if([](StanParticle* bullet) {
		if (bullet->IsDead()) {

			delete bullet;

			return true;
		}
		return false;
		});
}

void StanEmitter::Update(bool frag)
{
	// デスフラグの立ったパーティクルを削除
	particles_.remove_if([](StanParticle* bullet) {
		if (bullet->IsDead()) {

			delete bullet;

			return true;
		}
		return false;
		});

	
	if (frag)
	{
		timer_++;

		if ((timer_ <= 1 or timer_ % interval_ == 0) && timer_ <= 60)
		{
			Emit();
		}

	}
	else
	{
		timer_ = 0;
	}

	// パーティクル更新
	for (auto& particle : particles_) 
	{
		particle->Update();
	}
}

void StanEmitter::Draw()
{
	for (auto& particle : particles_)
	{
		particle->Draw();
	}
}

void StanEmitter::Emit()
{
	pos_.x = float(rand() % int(size_.x) + int(playerPos_.x - size_.x / 2));
	pos_.y = float(rand() % int(size_.y) + int(playerPos_.y - size_.y / 2));
	pos_.z = playerPos_.z;
	

	// 弾を生成し、初期化
	StanParticle* newParticle = new StanParticle();

	newParticle->SetPlayerPos(pos_);
	newParticle->Initialize();
	// 弾を登録する
	particles_.push_back(newParticle);
}
