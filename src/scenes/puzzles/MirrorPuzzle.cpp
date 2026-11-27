
#include "MirrorPuzzle.h"
#include "../../core/AssetManager.h"
#include "../../utilities/GameConstants.h"
#include "../../entities/items/door.h"
#include "../../entities/items/key.h"
#include "../../core/DoorManager.h"
#include "../../core/Inventory.h"
#include "../../core/DialogueManager.h"
#include "../../core/SceneManager.h"
#include "imgui.h"
#include <cstdio>
#include <string>
#include <utility>


void MirrorPuzzle::Init()
{
    if (!piezas.empty())
        return;

    AssetManager* assets = AssetManager::Instance();

    for (int i = 0; i < N_PIECES; i++)
    {
        PiezaPuzle* p = Create<PiezaPuzle>(puzzlePos[i]);
        p->SetCurrentPos(puzzlePos[i]);
        p->SetTexture(assets->GetTexture(Textures::MIRRORS[i]));
        piezas.push_back(p);
    }

    for (int i = 0; i < N_PIECES; i++)
        m_board[i] = i;

    selectedSlot = 0;
    LoadDefaults();
    Layout();
}


void MirrorPuzzle::LoadDefaults()
{
    m_origin = PUZZLE_ORIGIN;
    m_separation = PUZZLE_SEPARATION;

    for (int i = 0; i < N_PIECES; i++)
        m_offsets[i] = PIECES_OFFSET[i];
}


void MirrorPuzzle::SetPiecePos(int pieceIndex)
{
    SetPiecePos(pieceIndex, pieceIndex);
}


void MirrorPuzzle::SetPiecePos(int pieceIndex, int slotIndex)
{
    if (pieceIndex < 0 || pieceIndex >= (int)piezas.size() ||
        slotIndex < 0 || slotIndex >= N_PIECES)
    {
        TraceLog(LOG_WARNING,
            "MirrorPuzzle::SetPiecePos: indices fuera de rango (pieza %d, casilla %d)",
            pieceIndex, slotIndex);
        return;
    }

    const Vector2& celda = puzzlePos[slotIndex];

    Texture2D textura = piezas[pieceIndex]->GetTexture();

    piezas[pieceIndex]->SetCurrentPos(celda);

    const float width = textura.width / 5.0f;
    const float height = textura.height / 5.0f;

    // El centro pertenece a la casilla, no a la pieza
    const float centerX =
        m_origin.x + m_separation.x * celda.x + m_offsets[slotIndex].x;

    const float centerY =
        m_origin.y + m_separation.y * celda.y + m_offsets[slotIndex].y;

    piezas[pieceIndex]->SetRect(Rectangle{
        centerX - width / 2.0f,
        centerY - height / 2.0f,
        width,
        height
        });
}

void MirrorPuzzle::Layout()
{
    for (int slot = 0; slot < N_PIECES && slot < (int)piezas.size(); slot++)
        SetPiecePos(m_board[slot], slot);

    SwapSlots(0, 3, true);
    SwapSlots(2, 4, true);
    SwapSlots(4, 3, true);
    SwapSlots(5, 1, true);
}


int MirrorPuzzle::Neighbor(int slot, int dc, int dr) const
{
    const int col = slot % cols + dc;
    const int row = slot / cols + dr;

    if (col < 0 || col >= cols || row < 0 || row >= fils)
        return -1;

    return row * cols + col;
}


void MirrorPuzzle::SwapSlots(int slotA, int slotB, bool scramble)
{
    std::swap(m_board[slotA], m_board[slotB]);

    SetPiecePos(m_board[slotA], slotA);
    SetPiecePos(m_board[slotB], slotB);

    if(!scramble)
    CheckCorrect();
}


void MirrorPuzzle::OnEnter()
{

}

void MirrorPuzzle::CheckCorrect()
{
    bool c = true;
    for (auto p : piezas) {
        if (!p->IsCorrect()) {
            c = false;
            break;
        }
    }
    correct = c;
    if (c) {
        OnResolve();
    }
}

void MirrorPuzzle::OnResolve()
{
    DoorManager* dm = DoorManager::Instance();
    AssetManager* assets = AssetManager::Instance();

    Key* k2 = dm->GetKey(1);
    k2->SetTexture(assets->GetTexture(Textures::ITEM_KEY_2));
    k2->SetRect(Rectangle{ GameConstants::kScreenWidth/2, GameConstants::kScreenHeight / 2, 80.0f, 80.0f });
    k2->AddCallback([k2]() {
        k2->ResetCallbacks();
        Inventory::Instance()->AddObject(k2, FileName::KEY_INFO);
        });
    Adopt(k2);
}


void MirrorPuzzle::Update(float dt)
{
    Scene::Update(dt);

    if (correct) return;

    if (IsKeyPressed(KEY_T))
        selectionMode = !selectionMode;

    int dc = 0, dr = 0;

    if (IsKeyPressed(KEY_W))      dr = -1;
    else if (IsKeyPressed(KEY_S)) dr = 1;
    else if (IsKeyPressed(KEY_A)) dc = -1;
    else if (IsKeyPressed(KEY_D)) dc = 1;

    if (dc == 0 && dr == 0)
        return;

    const int destino = Neighbor(selectedSlot, dc, dr);

    if (destino < 0)
        return;

    if (selectionMode)
    {
        selectedSlot = destino;
    }
    else
    {
        SwapSlots(selectedSlot, destino);
        selectedSlot = destino;
    }
}


void MirrorPuzzle::OnExit()
{

}


void MirrorPuzzle::Draw()
{
    Scene::Draw();

    if (piezas.empty() || correct)
        return;

    // Marco alrededor de la pieza seleccionada
    const PiezaPuzle* seleccionada = piezas[m_board[selectedSlot]];

    DrawRectangleLinesEx(
        seleccionada->GetRect(),
        4.0f,
        selectionMode ? YELLOW : ORANGE
    );

    DrawText(
        selectionMode ? "Modo: seleccion (T: cambiar)" : "Modo: intercambio (T: cambiar)",
        20, 60, 24, RAYWHITE
    );

    DrawText("WASD: mover", 20, 90, 20, LIGHTGRAY);
}


void MirrorPuzzle::Release()
{
    // Los objetos los registra y los destruye Scene (Create)
}


void MirrorPuzzle::DrawDebugUI()
{
    ImGui::Separator();
    ImGui::Text("Puzzle de espejos");

    // Layout solo se llama cuando algo cambia
    bool cambiado = false;

    cambiado |= ImGui::DragFloat2("Origen", &m_origin.x, 1.0f);

    cambiado |= ImGui::DragFloat2(
        "Separacion",
        &m_separation.x,
        1.0f,
        0.0f,
        600.0f
    );

    if (ImGui::TreeNode("Offset por pieza"))
    {
        for (int i = 0; i < N_PIECES; i++)
        {
            char etiqueta[32];

            std::snprintf(
                etiqueta,
                sizeof(etiqueta),
                "Pieza %d",
                i + 1
            );

            cambiado |= ImGui::DragFloat2(
                etiqueta,
                &m_offsets[i].x,
                1.0f
            );
        }

        ImGui::TreePop();
    }

    if (ImGui::Button("Recolocar piezas"))
        cambiado = true;

    ImGui::SameLine();

    if (ImGui::Button("Restaurar valores fijos"))
    {
        LoadDefaults();
        cambiado = true;
    }

    // Copiar la configuración actual como código
    if (ImGui::Button("Copiar configuracion"))
    {
        char buf[128];
        std::string texto;

        std::snprintf(
            buf,
            sizeof(buf),
            "constexpr Vector2 PUZZLE_ORIGIN = { %.1ff, %.1ff };\n",
            m_origin.x,
            m_origin.y
        );

        texto += buf;

        std::snprintf(
            buf,
            sizeof(buf),
            "constexpr Vector2 PUZZLE_SEPARATION = { %.1ff, %.1ff };\n",
            m_separation.x,
            m_separation.y
        );

        texto += buf;

        texto += "constexpr Vector2 PIECES_OFFSET[N_PIECES] = {\n    ";

        for (int i = 0; i < N_PIECES; i++)
        {
            std::snprintf(
                buf,
                sizeof(buf),
                "{%.1ff, %.1ff}%s",
                m_offsets[i].x,
                m_offsets[i].y,
                i + 1 < N_PIECES ? ", " : ""
            );

            texto += buf;
        }

        texto += "\n};\n";

        ImGui::SetClipboardText(texto.c_str());
    }

    if (cambiado)
        Layout();
}
