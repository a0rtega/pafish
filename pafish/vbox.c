
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "vbox.h"

typedef char * string;

int vbox_reg_key1() {
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
            if (strstr(value, "VBOX") != NULL) {
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

int vbox_reg_key2() {
    HKEY regkey;
    LONG retu;
    char value[1024];
    int i;
    DWORD size;
    
    size = sizeof(value);
    retu = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\Description\\System", 0, KEY_READ, &regkey);
    if (retu == ERROR_SUCCESS) {
        retu = RegQueryValueEx(regkey, "SystemBiosVersion", NULL, NULL, (BYTE*)value, &size);
        if (retu == ERROR_SUCCESS) {
            for (i = 0; i < strlen(value); i++) { /* case-insensitive */
                value[i] = toupper(value[i]);
            }
            if (strstr(value, "VBOX") != NULL) {
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

int vbox_reg_key3() {
    HKEY regkey;
    LONG retu;
    retu = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Oracle\\VirtualBox Guest Additions", 0, KEY_READ, &regkey);
    if (retu == ERROR_SUCCESS) {
        return 0;
    }
    else {
        return 1;
    }
}

int vbox_reg_key4() {
    HKEY regkey;
    LONG retu;
    char value[1024];
    int i;
    DWORD size;
    
    size = sizeof(value);
    retu = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\Description\\System", 0, KEY_READ, &regkey);
    if (retu == ERROR_SUCCESS) {
        retu = RegQueryValueEx(regkey, "VideoBiosVersion", NULL, NULL, (BYTE*)value, &size);
        if (retu == ERROR_SUCCESS) {
            for (i = 0; i < strlen(value); i++) { /* case-insensitive */
                value[i] = toupper(value[i]);
            }
            if (strstr(value, "VIRTUALBOX") != NULL) {
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

int vbox_sysfile1() {
    DWORD ret;
    
    const int count = 4;
    string strs[count];
    int res = 1;
    char message[200];
    int i=0;
    
    strs[0] = "C:\\WINDOWS\\system32\\drivers\\VBoxMouse.sys";
    strs[1] = "C:\\WINDOWS\\system32\\drivers\\VBoxGuest.sys";
    strs[2] = "C:\\WINDOWS\\system32\\drivers\\VBoxSF.sys";
    strs[3] = "C:\\WINDOWS\\system32\\drivers\\VBoxVideo.sys";


    for (i=0; i < count; i++){
        sprintf(message, "[*] Looking for %s ... ", strs[i]);
        printf(message);
        ret = GetFileAttributes(strs[i]);
        if (ret != INVALID_FILE_ATTRIBUTES) {
            sprintf(message, "VirtualBox traced using file %s", strs[i]);
            write_log(message);
            print_traced();
            write_trace("hi_virtualbox");
            res = 0;
        }
    }
    
    return res;
    
}
