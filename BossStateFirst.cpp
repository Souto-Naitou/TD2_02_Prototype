#include "BossStateFirst.h"

#include "./Objects/Boss/Boss.h"
#include "BossStateSecond.h"

BossStateFirst::BossStateFirst(Boss* _pBoss) : BaseBossState("State First",_pBoss)
{

}

void BossStateFirst::Update()
{
	if (pBoss_->GetBossHP()<120) {

		pBoss_->ChangeState(std::make_unique<BossStateSecond>(pBoss_));

	}
}
