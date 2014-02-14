
#include <winsock2.h>
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <iphlpapi.h>
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

/**
* VirtualBox Driver files in windows/system32
**/
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
            sprintf(message, "VirtualBox traced using driver file %s", strs[i]);
            write_log(message);
            print_traced();
            write_trace("hi_virtualbox");
            res = 0;
        }
    }
    
    return res;
    
}

/**
* VirtualBox files in windows/system32
**/
int vbox_sysfile2() {
    DWORD ret;
    
    const int count = 12;
    string strs[count];
    int res = 1;
    char message[200];
    int i=0;

    strs[0] = "C:\\WINDOWS\\system32\\vboxdisp.dll";
    strs[1] = "C:\\WINDOWS\\system32\\vboxhook.dll";
    strs[2] = "C:\\WINDOWS\\system32\\vboxmrxnp.dll";
    strs[3] = "C:\\WINDOWS\\system32\\vboxogl.dll";
    strs[4] = "C:\\WINDOWS\\system32\\vboxoglarrayspu.dll";
    strs[5] = "C:\\WINDOWS\\system32\\vboxoglcrutil.dll";
    strs[6] = "C:\\WINDOWS\\system32\\vboxoglerrorspu.dll";
    strs[7] = "C:\\WINDOWS\\system32\\vboxoglfeedbackspu.dll";
    strs[8] = "C:\\WINDOWS\\system32\\vboxoglpackspu.dll";
    strs[9] = "C:\\WINDOWS\\system32\\vboxoglpassthroughspu.dll";
    strs[10] = "C:\\WINDOWS\\system32\\vboxservice.exe";
    strs[11] = "C:\\WINDOWS\\system32\\vboxtray.exe";

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
}

int vbox_mac() {
    WSADATA WSD;
    int res=1;
    char * message[200];
    char mac[6]={0};

    if(!WSAStartup(MAKEWORD(2,2),&WSD)){
        unsigned long alist_size=0;
        // getting the size of the adapter list
        int ret = GetAdaptersAddresses(AF_UNSPEC,GAA_FLAG_INCLUDE_PREFIX,0,0,&alist_size);
        if(ret==ERROR_BUFFER_OVERFLOW) {
            IP_ADAPTER_ADDRESSES* palist = (IP_ADAPTER_ADDRESSES*)LocalAlloc(LMEM_ZEROINIT,alist_size);
            if(palist) {
                ret=GetAdaptersAddresses(AF_UNSPEC,GAA_FLAG_INCLUDE_PREFIX,0,palist,&alist_size);
                IP_ADAPTER_ADDRESSES* ppalist=palist;

                while (ppalist){
                    if (ppalist->PhysicalAddressLength==0x6){
                        memcpy(mac,ppalist->PhysicalAddress,6);
                        if(mac[0]==0x08 && mac[1]==0x00 && mac[2]==0x27) { // VirtualBox mac starts with 08:00:27
                            write_log("VirtualBox traced using MAC starting with 08:00:27");
                            res = 0;
                            }
                        }

                    ppalist = ppalist->Next;
                    }
                LocalFree(palist);
            }
        }
        if (res == 0){
            print_traced();
            write_trace("hi_virtualbox");
        }

        WSACleanup();
    }

    return res;
}

