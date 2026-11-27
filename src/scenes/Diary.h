#pragma once
#include "Scene.h"
#include "raylib.h"
#include "imgui.h"
#include "../entities/Clickable.h"

enum NodeId {
    CONTEXTO = 0,
    LUCES_DE_BOHEMIA,
    ESPEJOS,
    ESPERPENTO,
    CANOURO,
    NUM_NODES
};

struct Node
{
    bool active = false;
    Vector2 pos{};
    Clickable button{};
};

struct Edge
{
    int nodeA, nodeB;
};

class Diary : public Scene
{
public:
    Diary(std::string name) : Scene(name) {}
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void DrawDebugUI() override;
    void Release() override;
    void OnEnter() override;

    void SetActiveNode(int id, bool b) { nodes[id].active = b; }
    int GetNodesSize() { return nodes.size(); }

private:
    void DrawFrame(const Texture& tex, int frame, int frames, ImVec2 size);
    Vector2 Center(int id) const { return { nodes[id].pos.x + 25, nodes[id].pos.y + 25 }; }
    void place(NodeId id, float x, float y);

    GameObject _pauseBackground{};
    Texture _diaryTexture{};
    Texture _diaryTextureImgui;

    const int FRAMES = 5;
    static constexpr float FRAME_DURATION = 0.12f;

    float scale = 1.0f;
    int   _animationFrame = 0;
    float _frameTimer = 0.0f;
    bool  _opening = true;
    bool  _closing = false;
    float w, h, frameW, frameH;

    std::vector<Node> nodes;
    std::vector<Edge> edges;
    Texture _nodeTexture;
};
