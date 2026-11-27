#pragma once
#include "../../entities/Draggable.h"

class PiezaPuzle : public GameObject
{
public:
	PiezaPuzle(Vector2 _finalPos) : GameObject(), posFinal(_finalPos){}

	bool IsCorrect() { return posActual.x == posFinal.x && posActual.y == posFinal.y;  }
private:
	int id;
	Vector2 posActual;
	Vector2 posFinal;
};