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

	// 通常弾更新
	for (auto& bullet : pBoss_->GetNormalBullets()) 
	{
		bullet->Update();
	}

	// 枕弾更新
	for (auto& bullet : pBoss_->GetPillows())
	{
		bullet->Update();
	}

	if (pBoss_->GetBossHP() < 60) 
	{

		pBoss_->ChangeState(std::make_unique<BossStateSecond>(pBoss_));

	}
}
