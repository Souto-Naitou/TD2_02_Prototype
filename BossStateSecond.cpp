#include "BossStateSecond.h"


#include "./Objects/Boss/Boss.h"
#include "BossStateThird.h"

BossStateSecond::BossStateSecond(Boss* _pBoss) : BaseBossState("State Second", _pBoss)
{
}

void BossStateSecond::Update()
{
	if (pBoss_->GetBossHP() < 80) {

		pBoss_->ChangeState(std::make_unique<BossStateThird>(pBoss_));

	}
}
