#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include "../entities/GameObject.h"
#include "../entities/Draggable.h"
#include "../entities/Clickable.h"

class Scene
{
public:
    Scene(const std::string& name) : m_name(name) {}
    virtual ~Scene() = default;

    virtual void Init() {}

    virtual void Update(float dt);
    virtual void Draw();
    virtual void DrawDebugUI() {}

    virtual void OnEnter() {}
    virtual void OnExit() {}

    virtual void Release() {}

    const std::string& GetName() const { return m_name; }

protected:
    template <typename T, typename... Args>
    T* Create(Args&&... args);

private:
    void Register(Draggable* d) { m_draggables.push_back(d);}
    void Register(Clickable* c){ m_clickables.push_back(c); }
    void Register(GameObject* go) { m_gameObjects.push_back(go); }

    std::string m_name;

    std::vector<std::unique_ptr<GameObject>> m_owned;

    std::vector<GameObject*> m_gameObjects;
    std::vector<Clickable*> m_clickables;
    std::vector<Draggable*> m_draggables;
};

template <typename T, typename... Args>
T* Scene::Create(Args&&... args)
{
    static_assert(std::is_base_of<GameObject, T>::value, "T debe heredar de GameObject");

    std::unique_ptr<T> obj(new T(std::forward<Args>(args)...));
    T* raw = obj.get();

    Register(raw);
    m_owned.push_back(std::move(obj));
    return raw;
}