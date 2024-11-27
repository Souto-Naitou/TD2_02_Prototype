#pragma once

#include "./Objects/.GameObject/GameObject.h"
#include "StanParticle.h"

#include <Object3d.h>
#include <Helper/Shape.h>
#include <memory>

class StanEmitter
{
public:
	
	void Initialize();

	void Finalize();

	void Update(bool frag);

	void Draw();

	void Emit();

public: // セッター

	void SetPlayerPos(Vector3 _position) { playerPos_ = _position; }

private:

	std::list<StanParticle*> particles_;

	Vector3 playerPos_{};
	Vector3 size_={5.0f,5.0f,5.0f};
	Vector3 pos_{};

	int timer_ = 0;
	int interval_ = 20;

};

