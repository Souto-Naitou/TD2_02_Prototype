#include "HPBar.h"

void HPBar::Initialize()
{
    barSprite_ = std::make_unique<Sprite>();
}

void HPBar::Finalize()
{
}

void HPBar::Update()
{
    Vector2 newScale = scale_.xy();
    newScale.x *= ratio_;
    barSprite_->SetSize(newScale);
    barSprite_->Update();
}

void HPBar::Draw()
{
}

void HPBar::Draw2D()
{
    if (barSprite_ == nullptr) return;

    barSprite_->Draw();
}

void HPBar::LoadBarSprite(const std::string& _fileName, Vector2 _position, Vector2 _anchor)
{
    barSprite_->Initialize(_fileName, _position, { 1.0f, 1.0f, 1.0f, 1.0f }, _anchor);
}

void HPBar::RegisterDebugWindow()
{
}

void HPBar::UnregisterDebugWindow()
{
}
