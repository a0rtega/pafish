
#include <windows.h>

#include "gensandbox.h"

int gensandbox_mouse_act() {
    POINT position1, position2;
    GetCursorPos(&position1);
    Sleep(1000); /* Sleep time */
    GetCursorPos(&position2);
    if ((position1.x == position2.x) && (position1.y == position2.y)) {
        /* No mouse activity during the sleep */
        return 0;
    }
    else {
        /* Mouse activity during the sleep */
        return 1;
    }
}
