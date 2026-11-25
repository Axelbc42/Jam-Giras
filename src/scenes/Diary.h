#pragma once
#include "Scene.h"
#include "raylib.h"
#include "imgui.h"
#include "../entities/Clickable.h"

class Diary : public Scene
{
public:
    Diary(std::string name) : Scene(name) {}
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void DrawDebugUI() override;
    void Release() override;

private:
    void DrawFrame(const Texture& tex, int frame, int frames, ImVec2 size);

    GameObject _pauseBackground{};
    Texture2D _diaryTexture{};

    const int WIDTH = 5108;
    const int HEIGHT = 767;
    const int FRAMES = 5;
    static constexpr float FRAME_DURATION = 0.12f;

    float scale = 1.4f;
    int   _animationFrame = 0;
    float _frameTimer = 0.0f;
    bool  _opening = true;
    bool  _closing = false;
};
