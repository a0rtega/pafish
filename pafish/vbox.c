
#include <winsock2.h>
#include <windows.h>
#include <winnetwk.h>
#include <string.h>
#include <stdio.h>
#include <iphlpapi.h>
#include <tlhelp32.h>
#include "vbox.h"

typedef char * string;

void ToUpper(unsigned char* Pstr) {
    char* P=(char*)Pstr;
    unsigned long length=strlen(P);
    unsigned long i;

    for(i=0;i<length;i++) P[i]=toupper(P[i]);

    return;
}

/**
* SCSI registry key check
**/
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

/**
* SystemBiosVersion registry key check
**/
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

/**
* GuestAdditions key check
**/
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

/**
* VideoBiosVersion key check
**/
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
* ACPI Regkey detection
**/
int vbox_reg_key5() {
    HKEY regkey;
    LONG retu;

    retu = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\ACPI\\DSDT\\VBOX__", 0, KEY_READ, &regkey);
    if (retu == ERROR_SUCCESS) {
        return 0;
    }
    else {
        return 1;
    }
}

/**
* IDE Registry key scanning
* http://0xmalware.blogspot.de/2013/10/cuckoo-sandbox-hardening-virtualbox.html
* https://twitter.com/waleedassar
**/
int vbox_reg_key6() {
    HKEY HK=0;
    int res=1;
    unsigned long i;
    char * message;
    DWORD ValType;
    long error;

    char* subkey="SYSTEM\\CurrentControlSet\\Enum\\IDE";
    if( (ERROR_SUCCESS==RegOpenKeyEx(HKEY_LOCAL_MACHINE,subkey,0,KEY_READ,&HK)) && HK ){
        unsigned long n_subkeys=0;
        unsigned long max_subkey_length=0;
        if(ERROR_SUCCESS==RegQueryInfoKey(HK,0,0,0,&n_subkeys,&max_subkey_length,0,0,0,0,0,0)){
            if(n_subkeys) { //Usually n_subkeys are 2
                char* pNewKey=(char*)LocalAlloc(LMEM_ZEROINIT,max_subkey_length+1);
                for(i=0;i<n_subkeys;i++) {  //Usually n_subkeys are 2
                    memset(pNewKey,0,max_subkey_length+1);
                    HKEY HKK=0;
                    if(ERROR_SUCCESS==RegEnumKey(HK,i,pNewKey,max_subkey_length+1)) {
                        if((RegOpenKeyEx(HK,pNewKey,0,KEY_READ,&HKK)==ERROR_SUCCESS)  && HKK) {
                            unsigned long nn=0;
                            unsigned long maxlen=0;
                            RegQueryInfoKey(HKK,0,0,0,&nn,&maxlen,0,0,0,0,0,0);
                            char* pNewNewKey=(char*)LocalAlloc(LMEM_ZEROINIT,maxlen+1);
                            if(RegEnumKey(HKK,0,pNewNewKey,maxlen+1)==ERROR_SUCCESS) {
                                HKEY HKKK=0;
                                if(RegOpenKeyEx(HKK,pNewNewKey,0,KEY_READ,&HKKK)==ERROR_SUCCESS) {
                                    unsigned long size=0xFFFF;
                                    unsigned char ValName[0x10000]={0};
                                    if(RegQueryValueEx(HKKK,"FriendlyName",0,0,ValName,&size)==ERROR_SUCCESS) {
                                        ToUpper(ValName);
                                        if(strstr((char*)ValName,"VBOX")) {
                                            message = (char*)LocalAlloc(LMEM_ZEROINIT,strlen(ValName)+200);
                                            if (message) {
                                                sprintf(message, "VBOX traced in IDE Registry based on FriendlyName containing VBOX %s ", ValName);
                                                write_log(message);
                                                LocalFree(message);
                                            }                                            
                                            res = 0;
                                        }
                                    }

                                    size = 0xFFFF;
                                    error = RegQueryValueEx(HKKK,"HardwareID",0,&ValType,ValName,&size);
                                    if(error==ERROR_SUCCESS) {
                                        if (ValType == REG_MULTI_SZ){
                                            char * sp = ValName;
                                            while(strlen(sp)){
                                                ToUpper(sp);
                                                if(strstr((char*)sp,"VBOX")) {
                                                    message = (char*)LocalAlloc(LMEM_ZEROINIT,strlen(sp)+200);
                                                    if (message) {
                                                        sprintf(message, "VBOX traced in IDE Registry based on HardwareID containing VBOX %s ", sp);
                                                        write_log(message);
                                                        LocalFree(message);
                                                    }                                            
                                                    res = 0;
                                                }
                                                sp = sp + strlen(sp) + 1;
                                            }
                                        }                                            
                                    }
                                    else{
                                        message = (char*)LocalAlloc(LMEM_ZEROINIT,200);
                                        sprintf(message, "%d", error);
                                        write_log(message);
                                        LocalFree(message);
                                    }
                                    RegCloseKey(HKKK);
                               }
                           }    
                           LocalFree(pNewNewKey);
                           RegCloseKey(HKK);
                       }
                   }
               }
               LocalFree(pNewKey);
           }
       }
       RegCloseKey(HK);
    }

    if (res == 0) {
        print_traced();
        write_trace("hi_virtualbox");
    }

    return res;
}

/**
* FADT ACPI Regkey detection
**/
int vbox_reg_key7() {
    HKEY regkey;
    LONG retu;

    retu = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\ACPI\\FADT\\VBOX__", 0, KEY_READ, &regkey);
    if (retu == ERROR_SUCCESS) {
        return 0;
    }
    else {
        return 1;
    }
}

/**
* RSDT ACPI Regkey detection
**/
int vbox_reg_key8() {
    HKEY regkey;
    LONG retu;

    retu = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\ACPI\\RSDT\\VBOX__", 0, KEY_READ, &regkey);
    if (retu == ERROR_SUCCESS) {
        return 0;
    }
    else {
        return 1;
    }
}

/**
* Service Regkey detection
**/
int vbox_reg_key9() {
    HKEY regkey;
    int res = 1;
    LONG retu;
    int i;
    const int count = 5;
    char message[200];

    string strs[count];
    strs[0] = "SYSTEM\\ControlSet001\\Services\\VBoxGuest";
    strs[1] = "SYSTEM\\ControlSet001\\Services\\VBoxMouse";
    strs[2] = "SYSTEM\\ControlSet001\\Services\\VBoxService";
    strs[3] = "SYSTEM\\ControlSet001\\Services\\VBoxSF";
    strs[4] = "SYSTEM\\ControlSet001\\Services\\VBoxVideo";

    for (i=0;i<count; i++){
        retu = RegOpenKeyEx(HKEY_LOCAL_MACHINE, strs[i], 0, KEY_READ, &regkey);
        if (retu == ERROR_SUCCESS) {
            sprintf(message, "VirtualBox traced registry key %s", strs[i]);
            write_log(message);
            res = 0;
        }
    }

    if (res == 0){
        print_traced();
        write_trace("hi_virtualbox");
    }

    return res;
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

/**
* NIC MAC check
**/
int vbox_mac() {
    WSADATA WSD;
    int res=1;
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

/**
* Checking for the VirtualBox pseudo device VBoxMiniRdrDN
* https://twitter.com/waleedassar
**/
int vbox_pseudodev() {
    int res=1;
    HANDLE h;

    h = CreateFile("\\\\.\\VBoxMiniRdrDN", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE){
        write_log("VBoxMiniRdrDN pseudo device detected");
        print_traced();
        write_trace("hi_virtualbox");
        res = 0;
        CloseHandle(h);
        }
 
    return res;
}

/**
* Checking for the VirtualBox pipe
* https://twitter.com/waleedassar
**/
int vbox_pipe() {
    int res=1;
    HANDLE h;

    h = CreateFile("\\\\.\\pipe\\VBoxTrayIPC", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE){
        write_log("VirtualBox VBoxTrayIPC pipe detected");
        print_traced();
        write_trace("hi_virtualbox");
        res = 0;
        CloseHandle(h);
        }
 
    return res;

}

/**
* Checking for Tray window
* https://twitter.com/waleedassar
**/
int vbox_traywindow() {
    int res=1;
    HWND h1;
    HWND h2;

    h1 = FindWindow("VBoxTrayToolWndClass", 0);
    h2 = FindWindow(0, "VBoxTrayToolWnd");

    if (h1 || h2){
        write_log("VirtualBox Tray tool window detected");
        print_traced();
        write_trace("hi_virtualbox");
        res = 0;
        }
 
    return res;
}


/**
* Checking network shared
* https://twitter.com/waleedassar
**/
int vbox_network_share() {
    int res=1;

    unsigned long pnsize=0x1000;

    char * provider=(char *)LocalAlloc(LMEM_ZEROINIT, pnsize);
    int retv = WNetGetProviderName(WNNC_NET_RDR2SAMPLE, provider, &pnsize);
    if (retv==NO_ERROR){
        if (lstrcmpi(provider, "VirtualBox Shared Folders") == 0){
            write_log("VirtualBox shared folder detected");
            print_traced();
            write_trace("hi_virtualbox");
            res = 0;
        }
    }
 
    return res;
}

/**
* Checking for virtual box processes
**/
int vbox_processes() {
    int res=1;
    HANDLE hpSnap;
    PROCESSENTRY32 pentry;

    hpSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if (hpSnap != INVALID_HANDLE_VALUE){
        pentry.dwSize = sizeof (PROCESSENTRY32);
    }

    if( !Process32First( hpSnap, &pentry ) ){
        CloseHandle(hpSnap);
        return 0;
    }

    do {
        if (lstrcmpi(pentry.szExeFile, "vboxservice.exe") == 0){
            write_log("vboxservice.exe process detected");
            res = 0;
        }
        if (lstrcmpi(pentry.szExeFile, "vboxtray.exe") == 0){
            write_log("vboxtray.exe process detected");
            res = 0;
        }
    } while( Process32Next( hpSnap, &pentry ) );
 
    if (res == 0){
        print_traced();
        write_trace("hi_virtualbox");
    }
    return res;
}
