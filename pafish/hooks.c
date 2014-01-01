
#include <windows.h>

#include "hooks.h"

/* Thx Inaki for this! (@virtualminds_es) */
int check_hook_DeleteFileW_m1() {
    DWORD *dwAddress = (DWORD *)DeleteFileW;
    BYTE *b = (BYTE *)dwAddress;
    if ((*b == 0xff) && (*(b+1) == 0x25)) {
        b++; b++;
        dwAddress = (DWORD *)b;
        DWORD *c = (DWORD *)(*dwAddress);
        BYTE *op = (BYTE *)*c;
        
        if ((*op == 0x8b) && (*(op+1) == 0xff)) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 1;
    }
}
