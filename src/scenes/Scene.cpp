#include "Scene.h"

void Scene::Update(float dt)
{
    Draggable* ocupado = nullptr;

    for (Draggable* d : m_draggables) {
        if (d->IsDragging()) { ocupado = d; break; }
    }

    if (ocupado) {
        ocupado->Update(dt);
    }
    else {
        for (auto it = m_draggables.rbegin(); it != m_draggables.rend(); ++it)
        {
            (*it)->Update(dt);
            if ((*it)->IsHovered()) { ocupado = *it; break; }
        }
    }

    if (ocupado) {
        for (Draggable* d : m_draggables)
            if (d != ocupado) d->ClearHover();
    }

    for (Clickable* c : m_clickables) {
        if (ocupado) c->ClearHover();
        else         c->Update(dt);
    }

    for (GameObject* go : m_gameObjects)
        go->Update(dt);
}

void Scene::Draw()
{
    for (GameObject* go : m_gameObjects) go->Draw();
    for (Clickable* c : m_clickables)    c->Draw();
    for (Draggable* d : m_draggables)    d->Draw();
}
