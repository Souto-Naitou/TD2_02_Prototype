#include "Skydome.h"

#include <ModelManager.h>

void Skydome::Initialize()
{
    /// モデルの読み込みと登録
    ModelManager::GetInstance()->LoadModel("skydome/tenkyurs.obj");

    /// Transformの初期化
    scale_ ={ 10.0f, 10.0f, 10.0f };
    position_ = { 0.0f, 0.0f, 0.0f };
    rotation_ = { 0.0f, 0.0f, 0.0f };

    object_ = std::make_unique<Object3d>();

    object_->Initialize("tenkyurs.obj");
    object_->SetSize(scale_);
    object_->SetRotate(rotation_);
    object_->SetPosition(position_);
}

void Skydome::Update()
{
    object_->Update();
}

void Skydome::Draw()
{
    object_->Draw();
}

void Skydome::Finalize()
{

}
