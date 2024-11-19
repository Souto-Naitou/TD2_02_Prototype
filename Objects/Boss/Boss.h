#pragma once

#include "../.GameObject/GameObject.h"
#include <Object3d.h>
#include <memory>
#include <Easing.h>
#include "Collision/Collider/Collider.h"
#include "Helper/Shape.h"

/// <summary>
/// ボス
/// </summary>
class Boss : public GameObject
{
public:
    Boss() = default;
    ~Boss() = default;

    void Initialize()   override;
    void Update()       override;
    void Draw()         override;
    void Finalize()     override;

    void RunSetMask();

private:
    std::unique_ptr<Object3d> object_ = nullptr;
    std::unique_ptr<Easing> easing_ = nullptr;
    Collider collider_;
    AABB aabb_;
};