#include "Player.h"

void Player::Initialize()
{
	// --- �J���� ---
	camera = new Camera();
	camera->SetRotate({ 0.3f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,4.0f,-10.0f });
	Object3dCommon::GetInstance()->SetDefaultCamera(camera);

	// --- 3D�I�u�W�F�N�g ---
	ModelManager::GetInstance()->LoadModel("plane.obj");

	for (uint32_t i = 0; i < 1; ++i) 
	{
		Object3d* object = new Object3d();
		object->Initialize(Object3dCommon::GetInstance());


		position_ = { 0.0f,0.0f,0.0f };
		object->SetPosition(position_);

		object->SetModel("plane.obj");

		object->SetSize({ 0.5f,0.5f,0.5f });

		object3ds.push_back(object);
	}

	velocity_ = { 0.05f,0.05f,0.0f };
}

void Player::Finalize()
{
	// �e�������
	delete camera;
	for (auto& obj : object3ds) 
	{
		delete obj;
	}

	Object3dCommon::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();
}

void Player::Update()
{
	//�J�����̍X�V
	camera->Update();


	for (uint32_t i = 0; i < object3ds.size(); ++i)
	{
		Object3d* obj = object3ds[i];
		obj->Update();

		// �ړ�����
		if (Input::GetInstance()->PushKey(DIK_W))
		{
			position_.y += velocity_.y;
		}
		if (Input::GetInstance()->PushKey(DIK_S))
		{
			position_.y -= velocity_.y;
		}
		if (Input::GetInstance()->PushKey(DIK_A))
		{
			position_.x -= velocity_.x;
		}
		if (Input::GetInstance()->PushKey(DIK_D))
		{
			position_.x += velocity_.x;
		}

		obj->SetPosition(position_);
	}

}

void Player::Draw()
{
	// �`��O����(Object)
	Object3dCommon::GetInstance()->PreDraw();

	// �� �� �� �� Draw ���������� �� �� �� ��

	for (auto& obj : object3ds)
	{
		obj->Draw();
	}

	// �� �� �� �� Draw ���������� �� �� �� ��
}
