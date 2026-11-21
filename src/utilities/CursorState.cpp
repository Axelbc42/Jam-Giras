#include "CursorState.h"

#include "raylib.h"

namespace
{
    bool s_wantHand = false;
    bool s_handShown = false;
}

namespace CursorState
{
    void BeginFrame() {
        s_wantHand = false;
    }

    void RequestHand(){
        s_wantHand = true;
    }

    void Apply(){
        if (s_wantHand == s_handShown)
            return;

        SetMouseCursor(s_wantHand ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);
        s_handShown = s_wantHand;
    }
}