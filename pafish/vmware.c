
#include <windows.h>
#include <string.h>

#include "vmware.h"

int vmware_reg_key1() {
    HKEY regkey;
    LONG retu;
    char value[1024];
    int i;
    DWORD size;
    
    size = sizeof(value);
    retu = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", 0, KEY_READ, &regkey);
    if (retu == ERROR_SUCCESS) {
        retu = RegQueryValueEx(regkey, "Identifier", NULL, NULL, (BYTE*)value, &size);
        if (retu == ERROR_SUCCESS) {
            for (i = 0; i < strlen(value); i++) { /* case-insensitive */
                value[i] = toupper(value[i]);
            }
            if (strstr(value, "VMWARE") != NULL) {
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
    else {
        return 1;
    }
}

int vmware_reg_key2() {
    HKEY regkey;
    LONG retu;
    retu = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\VMware, Inc.\\VMware Tools", 0, KEY_READ, &regkey);
    if (retu == ERROR_SUCCESS) {
        return 0;
    }
    else {
        return 1;
    }
}

int vmware_sysfile1() {
    DWORD ret;
    ret = GetFileAttributes("C:\\WINDOWS\\system32\\drivers\\vmmouse.sys");
    if (ret != INVALID_FILE_ATTRIBUTES) {
        return 0;
    }
    else {
        return 1;
    }
}

int vmware_sysfile2() {
    DWORD ret;
    ret = GetFileAttributes("C:\\WINDOWS\\system32\\drivers\\vmhgfs.sys");
    if (ret != INVALID_FILE_ATTRIBUTES) {
        return 0;
    }
    else {
        return 1;
    }
}
