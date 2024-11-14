#include "Boss.h"

#include <ModelManager.h>

void Boss::Initialize()
{
    /// モデルの読み込みと登録
    ModelManager::GetInstance()->LoadModel("cube.obj");

    /// Transformの初期化
    scale_ = { 1.0f, 1.0f, 1.0f };
    position_ = { 0.0f, 0.0f, 0.0f };
    rotation_ = { 0.0f, 0.0f, 0.0f };

    object_ = std::make_unique<Object3d>();

    object_->Initialize("cube.obj");
    object_->SetSize(scale_);
    object_->SetRotate(rotation_);
    object_->SetPosition(position_);
}

void Boss::Update()
{
    object_->Update();
}

void Boss::Draw()
{
    object_->Draw();
}

void Boss::Finalize()
{

}
