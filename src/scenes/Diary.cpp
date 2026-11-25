#include "Diary.h"
#include "../core/AssetManager.h"

void Diary::Init()
{
    _diaryTexture = AssetManager::Instance()->GetTexture(Textures::DIARY_FRAMES);
    _animationFrame = 0;
    _frameTimer = 0.0f;
    _opening = true;
    _closing = false;
}

void Diary::Update(float dt)
{
    if (_opening) {
        _frameTimer += dt;
        if (_frameTimer >= FRAME_DURATION) {
            _frameTimer = 0.0f;
            _animationFrame++;
            if (_animationFrame >= FRAMES - 1) {
                _animationFrame = FRAMES - 1;
                _opening = false;
            }
        }
    }
    //else if (_closing) {
    //    _frameTimer += dt;
    //    if (_frameTimer >= FRAME_DURATION) {
    //        _frameTimer = 0.0f;
    //        _animationFrame--;
    //        if (_animationFrame <= 0) {
    //            _animationFrame = 0;
    //            _closing = false;
    //        }
    //    }
    //}
}

void Diary::Draw()
{
    float frameW = _diaryTexture.width / (float)FRAMES;
    float frameH = (float)_diaryTexture.height;
    ImVec2 size(frameW * scale, frameH * scale);

    ImVec2 screen = ImGui::GetIO().DisplaySize;
    ImVec2 center(screen.x * 0.5f, screen.y * 0.45f);

    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(size);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoScrollbar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    if (ImGui::Begin("DiaryWindow", nullptr, flags)) {
        DrawFrame(_diaryTexture, _animationFrame, FRAMES, size);
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void Diary::Release()
{

}

void Diary::DrawDebugUI()
{

}

void Diary::DrawFrame(const Texture& tex, int frame, int frames, ImVec2 size)
{
    float fw = 1.0f / frames;

    ImVec2 uv0(frame * fw, 0.0f);
    ImVec2 uv1((frame + 1) * fw, 1.0f);

    ImGui::Image((ImTextureID)(intptr_t)tex.id, size, uv0, uv1);
}