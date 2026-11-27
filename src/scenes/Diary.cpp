#include "Diary.h"
#include "../core/AssetManager.h"
#include "../utilities/GameConstants.h"

void Diary::Init()
{
    _diaryTexture = AssetManager::Instance()->GetTexture(Textures::DIARY_FRAMES);
    frameW = _diaryTexture.width / FRAMES;
    frameH = _diaryTexture.height;
    w = frameW * scale;
    h = frameH * scale;

    _nodeTexture = AssetManager::Instance()->GetTexture(Textures::BUTON1);
    nodes.resize(NUM_NODES);
    for (Node& n : nodes) {
        n.button.SetTexture(_nodeTexture);
        n.button.SetSize(50, 50);
    }

    //200, 125 punto 0,0
    //550, 575 punto 1,1
    place(LUCES_DE_BOHEMIA, 325, 175);
    place(CONTEXTO,         225, 375);
    place(ESPERPENTO,       375, 350);
    place(ESPEJOS,          500, 325);
    place(CANOURO,          375, 500);

    nodes[LUCES_DE_BOHEMIA].button.AddClickDialogue(FileName::LUCES_DE_BOHEMIA_INFO);
    nodes[CONTEXTO].button.AddClickDialogue(FileName::CONTEXTO_INFO);
    nodes[ESPERPENTO].button.AddClickDialogue(FileName::ESPERPENTO_INFO);
    nodes[ESPEJOS].button.AddClickDialogue(FileName::ESPEJOS_INFO);
    nodes[CANOURO].button.AddClickDialogue(FileName::CANOURO_INFO);

    edges = {
        { CONTEXTO,         LUCES_DE_BOHEMIA },
        { LUCES_DE_BOHEMIA, ESPERPENTO },
        { ESPERPENTO,       ESPEJOS },
        { LUCES_DE_BOHEMIA, ESPEJOS },
        { CANOURO,          CONTEXTO },
        { CANOURO,          ESPERPENTO },
    };
}

void Diary::OnEnter() {
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
    for (Node& n : nodes)
        n.button.Update(dt);
}

void Diary::Draw()
{
    Rectangle src = { _animationFrame * frameW, 0.0f, frameW, frameH };
    Rectangle dst = { GameConstants::kScreenWidth * 0.5f - w * 0.5f, GameConstants::kScreenHeight * 0.5f - h * 0.55f, w, h };
    DrawTexturePro(_diaryTexture, src, dst, { 0, 0 }, 0.0f, WHITE);

    if (!_opening) {
        for (const Edge& e : edges) {
            if (nodes[e.nodeA].active || nodes[e.nodeB].active)
                DrawLineEx(Center(e.nodeA), Center(e.nodeB), 3.0f, DARKBROWN);
        }

        for (Node& n : nodes) {
            n.button.Draw();
        }
    }
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

void Diary::place(NodeId id, float x, float y) {
    nodes[id].pos = { x, y };
    nodes[id].button.SetPosition(x, y);
}
