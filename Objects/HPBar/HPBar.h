#pragma once

#include <Objects/.GameObject/GameObject.h>
#include <Sprite.h>
#include <string>
#include <Vector3.h>
#include <memory>

class HPBar : public GameObject
{
public:
    HPBar() = default;
    ~HPBar() = default;

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize() override;

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw() override;

    void Draw2D();

    void LoadBarSprite(const std::string& _fileName, Vector2 _position, Vector2 _anchor);

    void RegisterDebugWindow() override;
    void UnregisterDebugWindow() override;

public:
    void SetRatio(float _ratio) { ratio_ = _ratio; }


private:
    std::unique_ptr<Sprite> barSprite_ = nullptr;
    float ratio_ = 1.0f;
};