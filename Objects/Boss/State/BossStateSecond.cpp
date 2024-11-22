#include "BossStateSecond.h"


#include "../Boss.h"
#include "BossStateThird.h"

BossStateSecond::BossStateSecond(Boss* _pBoss) : BaseBossState("State Second", _pBoss)
{
	// 通常攻撃発生ファイル読み込み
	pBoss_->LoadNormalAttackPopData();

	// 枕攻撃発生ファイル読み込み
	pBoss_->LoadPillowPopData();
}

void BossStateSecond::Attack()
{
	// NormalAttack();
	pBoss_->UpdateNormalAttackPopCommands();

	// PillowAttack();
	pBoss_->UpdatePillowPopCommands();

	if (pBoss_->GetBossHP() < 80) {

		pBoss_->ChangeState(std::make_unique<BossStateThird>(pBoss_));

	}
}
