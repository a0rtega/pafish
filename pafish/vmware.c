
#include <windows.h>

int vmtools_detect_work_dir() {
    DWORD ret;
    ret = GetFileAttributes("C:\\Program Files\\VMware\\VMware Tools");
    if (ret != INVALID_FILE_ATTRIBUTES) {
        return 0;
    }
    else {
        return 1;
    }
}
