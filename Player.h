#pragma once

#include <Framework.h>
#include <Object3d.h>

class Player
{
public:
	// ������
	void Initialize();

	// �I��
	void Finalize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private: // �����o�ϐ�
	// �J����
	Camera* camera = nullptr;

	// 3D�I�u�W�F�N�g
	std::vector<Object3d*> object3ds;

	// �ʒu
	Vector3 position_{};

	// ���x
	Vector3 velocity_{};

};

