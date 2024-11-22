#include "Player.h"


void Player::Initialize()
{
    // --- 3Dオブジェクト ---
    ModelManager::GetInstance()->LoadModel("plane.obj");

    objectName_ = "Player";

    object_ = std::make_unique<Object3d>();
    object_->Initialize("plane.obj");

    position_ = { 0.0f,0.0f,0.0f };
    object_->SetPosition(position_);

    // 仮置き
    object_->SetSize({ 0.5f,0.5f,0.5f });

    this->RegisterDebugWindow();
}

void Player::Finalize()
{
    // 各解放処理

	for (auto& bullet : bullets_) {
		bullet->SetIsDead(true);
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

    this->UnregisterDebugWindow();
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

    moveVelocity_ = {};
    // 移動処理
    if (Input::GetInstance()->PushKey(DIK_W))
    {
        moveVelocity_ += playerForward * moveSpeed_;
    }
    if (Input::GetInstance()->PushKey(DIK_S))
    {
        moveVelocity_ += -playerForward * moveSpeed_;
    }
    if (Input::GetInstance()->PushKey(DIK_A))
    {
        moveVelocity_ += -playerRight * moveSpeed_;
    }
    if (Input::GetInstance()->PushKey(DIK_D))
    {
        moveVelocity_ += playerRight * moveSpeed_;
    }


    position_ += moveVelocity_;

    /// モデルに座標をセット
    object_->SetPosition(position_);

    rotation_.y -= mousePosDiff_.x * 0.001f;
	object_->SetRotate(rotation_);
    CameraFollow();

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

            // 弾を登録する
            bullets_.push_back(newBullet);

            countCoolDownFrame_ = kShootCoolDownFrame_;
        }
    }
    countCoolDownFrame_--;
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
