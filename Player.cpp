#include "Player.h"


void Player::Initialize()
{
	// --- 3Dオブジェクト ---
	ModelManager::GetInstance()->LoadModel("plane.obj");

	object_ = std::make_unique<Object3d>();
	object_->Initialize("plane.obj");

	position_ = { 0.0f,0.0f,0.0f };
	object_->SetPosition(position_);

	// 仮置き
	object_->SetSize({ 0.5f,0.5f,0.5f });


	velocity_ = { 0.05f,0.05f,0.0f };
}

void Player::Finalize()
{
	// 各解放処理

	for (auto& bullet : bullets_) {
		bullet->Finalize();
		//delete bullet;
	}

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			//bullet->Finalize();
			return true;
		}
		return false;
		});

	ModelManager::GetInstance()->Finalize();
}

void Player::Update()
{

	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			//bullet->Finalize();
			return true;
		}
		return false;
		});



	object_->Update();

	// 移動処理
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
	object_->SetPosition(position_);


	// 攻撃
	Attack();

	// 弾更新
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Draw()
{
	object_->Draw();


	// 弾描画
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}

}

void Player::Attack()
{
	if (Input::GetInstance()->PushKey(DIK_SPACE))
	{
		//for (uint32_t i = 0; i < 1; ++i)
		//{
		//    Object3d* object = new Object3d();
		//    object->Initialize("cube.obj");

		//    // 速度ベクトルを自機の向きに合わせて回転させる
		//    bltVelocity = TransformNormal(bltVelocity, worldTransformBlock.matWorld);
		//}

		bltVelocity_ = { 0.0f,0.0f,0.1f };

		if (bltCoolTime_ <= 0)
		{
			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();

			newBullet->SetPosition(position_);
			newBullet->Initialize();
			newBullet->SetVelocity(bltVelocity_);

			// 弾を登録する
			bullets_.push_back(newBullet);

			bltCoolTime_ = kBltCoolTime;
		}
	}
	bltCoolTime_--;
}