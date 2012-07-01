
#include <windows.h>

int vbox_gadd_reg_key() {
    HKEY res;
    LONG retu;
    /* We just try to detect if VirtualBox Guest Additions are installed looking at
       the RegKey */
    retu = RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Oracle\\VirtualBox Guest Additions", &res);
    if (retu == ERROR_SUCCESS) {
        return 0;
    }
    else {
        return 1;
    }
}
