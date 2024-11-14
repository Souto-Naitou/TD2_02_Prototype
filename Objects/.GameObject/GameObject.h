#pragma once

#include <Vector3.h>

class GameObject
{
public:
    GameObject() {}
    virtual ~GameObject() {}

    virtual void    Initialize()    = 0;
    virtual void    Update()        = 0;
    virtual void    Draw()          = 0;
    virtual void    Finalize()      = 0;


public: /// Getter
    virtual Vector3 GetScale()              const           { return scale_; };
    virtual Vector3 GetRotation()           const           { return rotation_; };
    virtual Vector3 GetPosition()           const           { return position_; };


public: /// Setter
    virtual void    SetScale(const Vector3& _scale)         { scale_ = _scale; };
    virtual void    SetRotation(const Vector3& _rotation)   { rotation_ = _rotation; };
    virtual void    SetPosition(const Vector3& _position)   { position_ = _position; };


protected: /// Member
    Vector3 scale_;
    Vector3 rotation_;
    Vector3 position_;
};