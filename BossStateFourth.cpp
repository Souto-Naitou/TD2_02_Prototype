#include "BossStateFourth.h"

#include "./Objects/Boss/Boss.h"

BossStateFourth::BossStateFourth(Boss* _pBoss) : BaseBossState("State Fourth", _pBoss)
{
	// 月攻撃発生ファイル読み込み
	pBoss_->LoadMoonPopData();
}

void BossStateFourth::Attack()
{
	// NormalAttack();
	pBoss_->UpdateMoonPopCommands();
}
