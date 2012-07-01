
#include <windows.h>

int sboxie_detect_sbiedll() {
    if (GetModuleHandle("sbiedll.dll") != NULL) {
        return 0;
    }
    else {
        return 1;
    }
}

int sboxie_detect_work_dir() {
    DWORD ret;
    ret = GetFileAttributes("C:\\Sandbox");
    if (ret != INVALID_FILE_ATTRIBUTES) {
        return 0;
    }
    else {
        return 1;
    }
}
