#pragma once
#include "../Scene.h"
#include "raylib.h"
#include "PiezaPuzle.h"

constexpr int N_PIECES = 6;
constexpr Vector2 puzzlePos[] = { {0,0}, {1,0}, {0,1}, {1,1}, {0,2}, {1,2} };   // (columna, fila)

constexpr Vector2 PUZZLE_ORIGIN = { 427.0f, 180.0f };   
constexpr Vector2 PUZZLE_SEPARATION = { 100.0f, 100.0f };  
constexpr float   PIECE_SIZE = 200.0f;             
constexpr Vector2 PIECES_OFFSET[N_PIECES] = {               
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
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

    int Neighbor(int slot, int dc, int dr) const;
    void SwapSlots(int slotA, int slotB);

    int cols = 2, fils = 3;
    std::vector<PiezaPuzle*> piezas; 

    // Valores ajustables desde el panel de depuracion
    Vector2 m_origin = PUZZLE_ORIGIN;
    Vector2 m_separation = PUZZLE_SEPARATION;
    float m_pieceSize = PIECE_SIZE;
    Vector2 m_offsets[N_PIECES] = {};

    int m_board[N_PIECES] = {};
    bool selectionMode = true;   
    int selectedSlot = 0;       
};