#pragma once

#include "../.GameObject/GameObject.h"
#include <Object3d.h>
#include <memory>

/// <summary>
/// É{ÉX
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

private:
    std::unique_ptr<Object3d> object_ = nullptr;
};