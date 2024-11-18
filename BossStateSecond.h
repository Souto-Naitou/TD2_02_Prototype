#pragma once

#include "BaseBossState.h"

class BossStateSecond : public BaseBossState
{
public:

	BossStateSecond(Boss* _pBoss);

	void Update();
};

