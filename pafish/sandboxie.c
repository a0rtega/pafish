
#include <windows.h>

#include "sandboxie.h"

int sboxie_detect_sbiedll() {
    if (GetModuleHandle("sbiedll.dll") != NULL) {
        return 0;
    }
    else {
        return 1;
    }
}
