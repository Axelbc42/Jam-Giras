#pragma once
#include "../Draggable.h"
#include "door.h"

class Key : public Draggable
{
public:
	Key(Door* _door) : door(_door){
		SetOnDrop([this]() {
            const Rectangle& r = GetRect();
            const Vector2 centro = { r.x + r.width / 2.0f, r.y + r.height / 2.0f };

            if (CheckCollisionPointRec(centro, door->GetRect())){
                door->OpenDoor();
                SetActive(false);
            }
            else{
                ReturnToStart();
            }
		});
	}

private:
	Door* door;
};