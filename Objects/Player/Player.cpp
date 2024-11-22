#include "Player.h"

#include "Collision/CollisionManager/CollisionManager.h"

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

    this->RegisterDebugWindow();


    collisionManager_ = CollisionManager::GetInstance();

    objectName_ = "Player";

    collider_.SetOwner(this);
    collider_.SetColliderID(objectName_);
    collider_.SetShapeData(&aabb_);
    collider_.SetShape(Shape::AABB);
    collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
    collider_.SetOnCollisionTrigger(std::bind(&Player::OnCollision, this));
    collisionManager_->RegisterCollider(&collider_);
}

void Player::Finalize()
{
    // 各解放処理

	for (auto& bullet : bullets_) {
		bullet->SetIsDead(true);
		bullet->Finalize();
	}

    bullets_.remove_if([](PlayerBullet* bullet) {
        if (bullet->IsDead()) {
            bullet->Finalize();
            delete bullet;
            return true;
        }
        return false;
        });

    ModelManager::GetInstance()->Finalize();

    this->UnregisterDebugWindow();


    collisionManager_->DeleteCollider(&collider_);
}

void Player::Update()
{

    //デスフラグの立った弾を削除
    bullets_.remove_if([](PlayerBullet* bullet) {
        if (bullet->IsDead()) {

            bullet->Finalize();
            delete bullet;

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

	if (Input::GetInstance()->TriggerKey(DIK_LEFT))
	{
		rotation_.z += 0.1f;
	}
	if (Input::GetInstance()->TriggerKey(DIK_RIGHT))
	{
		rotation_.z -= 0.1f;
	}
	object_->SetRotate(rotation_);

    // 攻撃
    Attack();

    aabb_.min = position_ - object_->GetSize();
    aabb_.max = position_ + object_->GetSize();
    collider_.SetPosition(position_);


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

            newBullet->RunSetMask();

            // 弾を登録する
            bullets_.push_back(newBullet);

            bltCoolTime_ = kBltCoolTime;
        }
    }
    bltCoolTime_--;
}

void Player::OnCollision()
{

}
