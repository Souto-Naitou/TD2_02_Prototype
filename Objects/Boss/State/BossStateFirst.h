#pragma once

#include "BaseBossState.h"

class BossStateFirst : public BaseBossState
{
public:

	BossStateFirst(Boss* _pBoss);

	void Attack();

};

