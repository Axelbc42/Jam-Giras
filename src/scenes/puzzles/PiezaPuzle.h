#pragma once
#include "../../entities/Draggable.h"

class PiezaPuzle : public GameObject
{
public:
	PiezaPuzle(Vector2 _finalPos) : GameObject(), posFinal(_finalPos){}

	bool IsCorrect() { return posActual.x == posFinal.x && posActual.y == posFinal.y;  }
	void SetCurrentPos(Vector2 pos) { posActual = pos;  }
private:
	int id;
	Vector2 posActual;
	Vector2 posFinal;
};