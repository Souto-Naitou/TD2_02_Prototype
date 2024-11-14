#include "Boss.h"

void Boss::Initialize()
{
    /// Transform‚Ì‰Šú‰»
    scale_ = { 1.0f, 1.0f, 1.0f };
    position_ = { 0.0f, 0.0f, 0.0f };
    rotation_ = { 0.0f, 0.0f, 0.0f };

    object_ = std::make_unique<Object3d>();
    object_->Initialize("cube.obj");
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
