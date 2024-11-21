#include "BossStateFirst.h"

#include "../Boss.h"
#include "BossStateSecond.h"

BossStateFirst::BossStateFirst(Boss* _pBoss) : BaseBossState("State First",_pBoss)
{
	// 通常攻撃発生ファイル読み込み
 	pBoss_->LoadNormalAttackPopData();

	// 枕攻撃発生ファイル読み込み
	pBoss_->LoadPillowPopData();
}

void BossStateFirst::Attack()
{
	// NormalAttack();
	pBoss_->UpdateNormalAttackPopCommands();

	// PillowAttack();
	pBoss_->UpdatePillowPopCommands();

	if (pBoss_->GetBossHP() < 120) {

		pBoss_->ChangeState(std::make_unique<BossStateSecond>(pBoss_));

	}
}
