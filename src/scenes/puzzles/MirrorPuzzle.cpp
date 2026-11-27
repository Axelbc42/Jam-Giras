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

	for (int i = 0; i < N_PIECES; i++) {
		PiezaPuzle* p = Create<PiezaPuzle>(puzzlePos[i]);
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
	m_pieceSize = PIECE_SIZE;
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
		TraceLog(LOG_WARNING, "MirrorPuzzle::SetPiecePos: indices fuera de rango (pieza %d, casilla %d)",
			pieceIndex, slotIndex);
		return;
	}

	const Vector2& celda = puzzlePos[slotIndex];

	piezas[pieceIndex]->SetRect(Rectangle{
		m_origin.x + m_separation.x * celda.x + m_offsets[pieceIndex].x,
		m_origin.y + m_separation.y * celda.y + m_offsets[pieceIndex].y,
		m_pieceSize,
		m_pieceSize });
}

void MirrorPuzzle::Layout()
{
	for (int slot = 0; slot < N_PIECES && slot < (int)piezas.size(); slot++)
		SetPiecePos(m_board[slot], slot);
}

int MirrorPuzzle::Neighbor(int slot, int dc, int dr) const
{
	const int col = slot % cols + dc;
	const int row = slot / cols + dr;

	if (col < 0 || col >= cols || row < 0 || row >= fils)
		return -1;
	return row * cols + col;
}

void MirrorPuzzle::SwapSlots(int slotA, int slotB)
{
	std::swap(m_board[slotA], m_board[slotB]);
	SetPiecePos(m_board[slotA], slotA);
	SetPiecePos(m_board[slotB], slotB);
}

void MirrorPuzzle::OnEnter()
{

}

void MirrorPuzzle::Update(float dt)
{
	Scene::Update(dt);

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

	if (selectionMode){
		selectedSlot = destino;                   
	}
	else{
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

	if (piezas.empty())
		return;

	// Marco alrededor de la pieza seleccionada: amarillo al seleccionar, naranja al intercambiar
	const PiezaPuzle* seleccionada = piezas[m_board[selectedSlot]];
	DrawRectangleLinesEx(seleccionada->GetRect(), 4.0f, selectionMode ? YELLOW : ORANGE);

	DrawText(selectionMode ? "Modo: seleccion (T: cambiar)" : "Modo: intercambio (T: cambiar)",
		20, 60, 24, RAYWHITE);
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

	// Layout solo se llama cuando algo cambia: si las piezas se arrastran,
	// recolocarlas en cada frame pelearia con el arrastre.
	bool cambiado = false;
	cambiado |= ImGui::DragFloat2("Origen", &m_origin.x, 1.0f);
	cambiado |= ImGui::DragFloat2("Separacion", &m_separation.x, 1.0f, 0.0f, 600.0f);
	cambiado |= ImGui::DragFloat("Tamano pieza", &m_pieceSize, 1.0f, 10.0f, 600.0f);

	if (ImGui::TreeNode("Offset por pieza"))
	{
		for (int i = 0; i < N_PIECES; i++)
		{
			char etiqueta[32];
			std::snprintf(etiqueta, sizeof(etiqueta), "Pieza %d", i + 1);
			cambiado |= ImGui::DragFloat2(etiqueta, &m_offsets[i].x, 1.0f);
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

	// Copia la configuracion actual como codigo, lista para pegar en MirrorPuzzle.h
	if (ImGui::Button("Copiar configuracion"))
	{
		char buf[128];
		std::string texto;

		std::snprintf(buf, sizeof(buf), "constexpr Vector2 PUZZLE_ORIGIN     = { %.1ff, %.1ff };\n", m_origin.x, m_origin.y);
		texto += buf;
		std::snprintf(buf, sizeof(buf), "constexpr Vector2 PUZZLE_SEPARATION = { %.1ff, %.1ff };\n", m_separation.x, m_separation.y);
		texto += buf;
		std::snprintf(buf, sizeof(buf), "constexpr float   PIECE_SIZE        = %.1ff;\n", m_pieceSize);
		texto += buf;

		texto += "constexpr Vector2 PIECES_OFFSET[N_PIECES] = {\n    ";
		for (int i = 0; i < N_PIECES; i++)
		{
			std::snprintf(buf, sizeof(buf), "{%.1ff, %.1ff}%s", m_offsets[i].x, m_offsets[i].y,
				i + 1 < N_PIECES ? ", " : "");
			texto += buf;
		}
		texto += "\n};\n";

		ImGui::SetClipboardText(texto.c_str());
	}

	if (cambiado)
		Layout();
}