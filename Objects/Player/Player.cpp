#include "Player.h"
#include <Helper/ImGuiTemplates/ImGuiTemplates.h>

#include "Collision/CollisionManager/CollisionManager.h"

void Player::Initialize()
{
    // --- 3Dオブジェクト ---
    ModelManager::GetInstance()->LoadModel("player/player.obj");

    // --- スプライト ---
    std::string textureFile[] = { "test/uvChecker.png","test/uvChecker.png" };
    for (uint32_t i = 0; i < 2; ++i) {
        Sprite* sprite = new Sprite();
        sprite->Initialize(textureFile[0], topClosePos_, { 1,1,1,1 }, { 0,0 });
        sprite->Initialize(textureFile[1], underClosePos_, { 1,1,1,1 }, { 0,0 });

        sprites.push_back(sprite);
    }

    object_ = std::make_unique<Object3d>();
    object_->Initialize("player.obj");

    position_ = { 0.0f,0.0f,0.0f };
    object_->SetPosition(position_);

    // 仮置き
    object_->SetSize({ 0.2f,0.2f,0.2f });

    this->RegisterDebugWindow();

    easing_ = std::make_unique<Easing>("CloseEye", Easing::EaseType::EaseOutBack);
    easing_->Initialize();

    // 状態異常タイムセット
    stanTimer_ = kStanTime_;
    narrowTimer_ = kNarrowTime_;

    // HPリセット
    hp_ = kMaxHp_;

    collisionManager_ = CollisionManager::GetInstance();

    objectName_ = "Player";

    collider_.SetOwner(this);
    collider_.SetColliderID(objectName_);
    collider_.SetShapeData(&aabb_);
    collider_.SetShape(Shape::AABB);
    collider_.SetAttribute(collisionManager_->GetNewAttribute(collider_.GetColliderID()));
    collider_.SetOnCollisionTrigger(std::bind(&Player::OnCollisionTrigger, this, std::placeholders::_1));
    collisionManager_->RegisterCollider(&collider_);

    // パーティクル
    pStanEmit_ = new StanEmitter();
    pStanEmit_->Initialize();
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

    for (Sprite* sprite : sprites) {
        delete sprite;
    }

    pStanEmit_->Finalize();

    collisionManager_->DeleteCollider(&collider_);
}

void Player::Update()
{
    for (uint32_t i = 0; i < 2; ++i)
    {
        if (i == 0)
        {
            sprites[i]->SetPosition(topMovePos_);
        }
        else
        {
            sprites[i]->SetPosition(underMovePos_);
        }

        Vector2 size = { 1600.0f,720.0f };
        sprites[i]->SetSize(size);

        Vector4 color = sprites[i]->GetColor();
        sprites[i]->SetColor(color);

        sprites[i]->Update();
    }

    //デスフラグの立った弾を削除
    bullets_.remove_if([](PlayerBullet* bullet) {
        if (bullet->IsDead()) {

            bullet->Finalize();
            delete bullet;

            return true;
        }
        return false;
        });

    if (Input::GetInstance()->TriggerKey(DIK_AT))
    {
        cursorVisible_ = !cursorVisible_;
        cursorLock_ = !cursorLock_;

        if (cursorVisible_)
        {
            ShowCursor(1);
        }
        else
        {
            ShowCursor(0);
        }
    }

    if (cursorLock_)
    {
        CalcCursorMove();
        SetCursorPos(1920 / 2, 1080 / 2);
    }

    object_->Update();

    Vector3 playerForward = { std::sinf(rotation_.y), 0.f, std::cosf(rotation_.y) };
    Vector3 playerRight = { std::cosf(rotation_.y), 0.f, -std::sinf(rotation_.y) };

    if (!isInertia_)
    {
        moveVelocity_ = {};
    }
    else
    {
        inertiaTimer_ -= kInertiaCount_;

        if (inertiaTimer_ < 0)
        {
            inertiaTimer_ = kInertiaTime_;
            isInertia_ = false;
        }
    }

    if (!isStan_)
    {
        // 移動処理
        if (Input::GetInstance()->PushKey(DIK_W))
        {
            moveVelocity_ += playerForward * moveSpeed_;
            moveVelocity_.z = min(moveVelocity_.z, kMaxVel_.z);
        }
        if (Input::GetInstance()->PushKey(DIK_S))
        {
            moveVelocity_ += -playerForward * moveSpeed_;
            moveVelocity_.z = max(moveVelocity_.z, -kMaxVel_.z);
        }
        if (Input::GetInstance()->PushKey(DIK_A))
        {
            moveVelocity_ += -playerRight * moveSpeed_;
            moveVelocity_.x = max(moveVelocity_.x, -kMaxVel_.x);
        }
        if (Input::GetInstance()->PushKey(DIK_D))
        {
            moveVelocity_ += playerRight * moveSpeed_;
            moveVelocity_.x = min(moveVelocity_.x, kMaxVel_.x);
        }

#ifdef _DEBUG
        // 月の判定確認用移動処理
        if (Input::GetInstance()->PushKey(DIK_UP))
        {
            moveVelocity_.y += moveSpeed_;
            moveVelocity_.y = max(moveVelocity_.y, -kMaxVel_.y);
        }
        if (Input::GetInstance()->PushKey(DIK_DOWN))
        {
            moveVelocity_.y += -moveSpeed_;
            moveVelocity_.y = min(moveVelocity_.y, kMaxVel_.y);
        }

#endif // _DEBUG



    }
    else if(isStan_)
    {
        stanTimer_ -= kStanCount_;
        moveVelocity_ = {};
        if (stanTimer_ < 0)
        {
            stanTimer_ = kStanTime_;
            isStan_ = false;
        }
    }

    position_ += moveVelocity_;

    /// モデルに座標をセット
    object_->SetPosition(position_);

    rotation_.y -= mousePosDiff_.x * 0.001f;
    object_->SetRotate(rotation_);
    CameraFollow();

    // 攻撃
    Attack();

    // 視野狭まる
    Narrow();

    aabb_.min = position_ - object_->GetSize();
    aabb_.max = position_ + object_->GetSize();
    collider_.SetPosition(position_);

    if (isHit_)
    {
        isHit_ = false;
    }

    // 弾更新
    for (auto& bullet : bullets_) {
        bullet->Update();
    }

    // パーティクル
    pStanEmit_->SetPlayerPos(position_);
    pStanEmit_->Update(isStan_);
}

void Player::Draw()
{
    object_->Draw();


    // 弾描画
    for (auto& bullet : bullets_)
    {
        bullet->Draw();
    }

    for (uint32_t i = 0; i < 2; ++i)
    {
       	sprites[i]->Draw();
    }

    pStanEmit_->Draw();
}

void Player::Attack()
{
    if (Input::GetInstance()->PushKey(DIK_SPACE))
    {
        /// プレイヤーの向きに合わせて弾の速度を変更
        Vector3 bulletVelocity = { std::sinf(rotation_.y), 0.f, std::cosf(rotation_.y) };;

        if (countCoolDownFrame_ <= 0)
        {
            // 弾を生成し、初期化
            PlayerBullet* newBullet = new PlayerBullet();

            newBullet->SetPosition(position_);
            newBullet->Initialize();
            newBullet->SetVelocity(bulletVelocity);

            newBullet->RunSetMask();
            collider_.SetMask(collisionManager_->GetNewMask(collider_.GetColliderID(), "PlayerBullet"));

            // 弾を登録する
            bullets_.push_back(newBullet);

            countCoolDownFrame_ = kShootCoolDownFrame_;
        }
    }
    countCoolDownFrame_--;
}

void Player::Narrow()
{
    if (isNarrow_)
    {
        easing_->Start();
        narrowTimer_ -= kNarrowCount_;

        // 上瞼的な
        topMovePos_.Lerp(topPos_, topClosePos_, easing_->Update());
        sprites[0]->SetPosition(topMovePos_);
        // 下瞼的な
        underMovePos_.Lerp(underPos_, underClosePos_, easing_->Update());
        sprites[1]->SetPosition(underMovePos_);

        if (narrowTimer_ < 0)
        {
            if (isClose_)
            {
                isClose_ = false;
                easing_->Reset();
            }

            // 上瞼的な
            topMovePos_.Lerp(topClosePos_, topPos_, easing_->Update());
            sprites[0]->SetPosition(topMovePos_);
            // 下瞼的な
            underMovePos_.Lerp(underClosePos_, underPos_, easing_->Update());
            sprites[1]->SetPosition(underMovePos_);

            if (easing_->GetIsEnd())
            {
                easing_->Reset();
                narrowTimer_ = kNarrowTime_;
                isNarrow_ = false;
                isClose_ = true;
            }
        }
    }
}


void Player::OnCollisionTrigger(const Collider* _other)
{
    if (_other->GetColliderID() != "BossMoon" && !isHit_)
    {
        hp_ -= 1;
    }
}

void Player::CameraFollow()
{
    Matrix4x4 playerWorld = Matrix4x4::AffineMatrix(scale_, rotation_, position_);
    Matrix4x4 cameraLocal = Matrix4x4::AffineMatrix({ 1.0f,1.0f,1.0f }, {}, cameraDistance_);

    Vector3 camPos_target = Transform(cameraDistance_, playerWorld);
    Vector3 camPos_current = mainCamera_->GetTranslate();

    cameraRotate_.y = rotation_.y;

    mainCamera_->SetRotate(cameraRotate_);

    Vector3 camPos_nextFrame = {};
    camPos_nextFrame.Lerp(camPos_current, camPos_target, camFollowMultiply_);
    mainCamera_->SetTranslate(camPos_nextFrame);
}

void Player::DebugWindow()
{
    Vector3 playerDirection = { std::sinf(rotation_.y), 0.f, std::cosf(rotation_.y) };
    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("Position", position_);
        ImGuiTemplate::VariableTableRow("Rotation", rotation_);
        ImGuiTemplate::VariableTableRow("Scale", scale_);
        ImGuiTemplate::VariableTableRow("PlayerDirection", playerDirection);
        ImGuiTemplate::VariableTableRow("HP", hp_);
    };

    ImGuiTemplate::VariableTable("Player", pFunc);

    ImGui::DragFloat3("Camera Rotate", &cameraRotate_.x, 0.01f);
    ImGui::DragFloat3("Camera Follow", &cameraDistance_.x, 0.01f);
    ImGui::Separator();
    ImGui::SliderFloat("Camera Follow Multiply", &camFollowMultiply_, 0.0f, 1.0f);
    ImGui::DragFloat2("MouseMoving", &mousePosDiff_.x, 0.01f);
}

void Player::CalcCursorMove()
{
    POINT mousePosCurrent;
    // マウス座標取得
    GetCursorPos(&mousePosCurrent);
    mousePosDiff_ =
    {
        static_cast<float>(960 - mousePosCurrent.x),
        static_cast<float>(540 - mousePosCurrent.y)
    };
}
