#include "BossStateThird.h"


#include "./Objects/Boss/Boss.h"
#include "BossStateFourth.h"

BossStateThird::BossStateThird(Boss* _pBoss) : BaseBossState("State Third", _pBoss)
{
}

void BossStateThird::Attack()
{
	if (pBoss_->GetBossHP() < 40) {

		pBoss_->ChangeState(std::make_unique<BossStateFourth>(pBoss_));

	}
}
