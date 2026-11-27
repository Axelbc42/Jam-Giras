
#pragma once
#include "../Scene.h"
#include "raylib.h"
#include "PiezaPuzle.h"

constexpr int N_PIECES = 6;
constexpr Vector2 puzzlePos[] = {
    {0,0}, {1,0},
    {0,1}, {1,1},
    {0,2}, {1,2}
}; 

constexpr Vector2 PUZZLE_ORIGIN = { 460.0f, 61.0f };
constexpr Vector2 PUZZLE_SEPARATION = { 86.0f, 181.0f };
constexpr Vector2 PIECES_OFFSET[N_PIECES] = {
    {0.0f, 0.0f}, {-19.0f, -13.0f}, {-9.0f, 64.0f}, {108.0f, -69.0f}, {14.0f, 46.0f}, {94.0f, -72.0f}
};

class MirrorPuzzle : public Scene
{
public:
    MirrorPuzzle(const std::string& name) : Scene(name) {}

    void Init() override;
    void Draw() override;
    void Release() override;
    void DrawDebugUI() override;
    void OnEnter() override;
    void Update(float dt) override;
    void OnExit() override;

    void SetPiecePos(int pieceIndex);
    void SetPiecePos(int pieceIndex, int slotIndex);

private:
    void LoadDefaults();
    void Layout();
    void CheckCorrect();

    int Neighbor(int slot, int dc, int dr) const;
    void SwapSlots(int slotA, int slotB);

    int cols = 2, fils = 3;
    std::vector<PiezaPuzle*> piezas;

    // Valores ajustables desde el panel de depuración
    Vector2 m_origin = PUZZLE_ORIGIN;
    Vector2 m_separation = PUZZLE_SEPARATION;
    Vector2 m_offsets[N_PIECES] = {};

    int m_board[N_PIECES] = {};
    bool selectionMode = true;
    int selectedSlot = 0;

    bool correct = false;
};
