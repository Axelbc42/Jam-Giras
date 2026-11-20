#pragma once
#include <string>

class Scene
{
public:
    Scene(std::string _name) : name(_name){}
    virtual ~Scene() = default;

    virtual void Init() {}
    virtual void Update(float dt) {}
    virtual void Draw() {}
    virtual void DrawDebugUI(){}

    virtual void OnEnter() {}
    virtual void OnExit() {}

    virtual void Release() {}

    std::string GetName() { return name; }

private:
    std::string name;
};