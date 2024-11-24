#include "BossStateFourth.h"

#include "../Boss.h"

BossStateFourth::BossStateFourth(Boss* _pBoss) : BaseBossState("State Fourth", _pBoss)
{
	// 通常攻撃発生ファイル読み込み
	pBoss_->LoadNormalAttackPopData();

	// 枕攻撃発生ファイル読み込み
	pBoss_->LoadPillowPopData();

	// 歌発生ファイル読み込み
	pBoss_->LoadSongPopData();

	// 月攻撃発生ファイル読み込み
	pBoss_->LoadMoonPopData();
}

void BossStateFourth::Attack()
{
	// NormalAttack();
	pBoss_->UpdateNormalAttackPopCommands();

	// PillowAttack();
	pBoss_->UpdatePillowPopCommands();

	// SongAttack();
	pBoss_->UpdateSongPopCommands();

	// NormalAttack();
	pBoss_->UpdateMoonPopCommands();
}
