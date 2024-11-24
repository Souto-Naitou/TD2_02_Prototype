#include "BossStateThird.h"


#include "../Boss.h"
#include "BossStateFourth.h"

BossStateThird::BossStateThird(Boss* _pBoss) : BaseBossState("State Third", _pBoss)
{
	// 通常攻撃発生ファイル読み込み
	//pBoss_->LoadNormalAttackPopData();

	// 枕攻撃発生ファイル読み込み
	//pBoss_->LoadPillowPopData();

	// 歌発生ファイル読み込み
	pBoss_->LoadSongPopData();
}

void BossStateThird::Attack()
{
	// NormalAttack();
	//pBoss_->UpdateNormalAttackPopCommands();

	// PillowAttack();
	//pBoss_->UpdatePillowPopCommands();

	// SongAttack();
	pBoss_->UpdateSongPopCommands();

	if (pBoss_->GetBossHP() < 40) {

		pBoss_->ChangeState(std::make_unique<BossStateFourth>(pBoss_));

	}
}
