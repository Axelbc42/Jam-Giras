#pragma once

class Scene
{
public:
    virtual ~Scene() = default;

    virtual void Init() {}
    virtual void Update(float dt) {}
    virtual void Draw() {}

    virtual void OnEnter() {}
    virtual void OnExit() {}

    virtual void Release() {}
};