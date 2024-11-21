#pragma once

#include <string>

class Boss;

class BaseBossState
{
public:

	BaseBossState(const std::string& _scene, Boss* _pBoss) : scene_(_scene), pBoss_(_pBoss) {};
	virtual ~BaseBossState();

	virtual void Attack() = 0;

protected:

	std::string scene_;

	Boss* pBoss_ = nullptr;

};

