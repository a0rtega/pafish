
#include <windows.h>

#include "wine.h"

int wine_detect_get_unix_file_name() {
    HMODULE k32;
    k32 = GetModuleHandle("kernel32.dll");
    if (k32 != NULL) {
        if (GetProcAddress(k32, "wine_get_unix_file_name") != NULL) {
            return 0;
        }
        else {
            return 1;
        }
    }
    else {
        return 1;
    }
}
