
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "common.h"

#include "debuggers.h"
#include "sandboxie.h"
#include "gensandbox.h"
#include "hooks.h"
#include "vbox.h"
#include "wine.h"
#include "vmware.h"
#include "qemu.h"

/*
  Pafish (Paranoid fish)
  
  All code from this project, including
  functions, procedures and the main program
  is licensed under GNU/GPL version 3.
  
  So, if you are going to use functions or
  procedures from this project to develop
  your malware, you have to release the
  source code as well :)
  
  - Alberto Ortega
  
*/

int main(int argc, char *argv[])
{
    char icon[] = "Blue fish icon thanks to http://www.fasticon.com/", winverstr[32], aux[1024];
    OSVERSIONINFO winver;
    
    write_log("Start");
    
    init_cmd_colors();
    print_header();
    
    winver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&winver);
    snprintf(winverstr, sizeof(winverstr), "%d.%d build %d", winver.dwMajorVersion, winver.dwMinorVersion, winver.dwBuildNumber);
    
    printf("[*] Windows version: %s\n", winverstr);
    snprintf(aux, sizeof(aux), "Windows version: %s", winverstr);
    write_log(aux);
    
    printf("[*] Running checks ...\n");
    
    /* Debuggers detection tricks */
    printf("\n[-] Debuggers detection\n");
    printf("[*] Using IsDebuggerPresent() ... ");
    if (debug_isdebuggerpresent() == 0) {
        write_log("Debugger traced using IsDebuggerPresent()");
        print_traced();
        write_trace("hi_debugger_isdebuggerpresent");
    }
    else {
        print_not_traced();
    }
    /* This is only working on MS Windows systems prior to Vista */
    if (winver.dwMajorVersion < 6) {
        printf("[*] Using OutputDebugString() ... ");
        if (debug_outputdebugstring() == 0) {
            write_log("Debugger traced using OutputDebugString()");
            print_traced();
            write_trace("hi_debugger_outputdebugstring");
        }
        else {
            print_not_traced();
        }
    }
    
    /* Generic sandbox detection tricks */
    printf("\n[-] Generic sandbox detection\n");
    printf("[*] Using mouse activity ... ");
    if (gensandbox_mouse_act() == 0) {
        print_traced();
        write_log("Sandbox traced using mouse activity");
        write_trace("hi_sandbox_mouse_act");
    }
    else {
        print_not_traced();
    }
    printf("[*] Checking username ... ");
    if (gensandbox_username() == 0) {
        print_traced();
        write_log("Sandbox traced by checking username");
        write_trace("hi_sandbox_username");
    }
    else {
        print_not_traced();
    }
    printf("[*] Checking file path ... ");
    if (gensandbox_path() == 0) {
        print_traced();
        write_log("Sandbox traced by checking file path");
        write_trace("hi_sandbox_path");
    }
    else {
        print_not_traced();
    }
    printf("[*] Checking if disk size <= 50GB ... ");
    if (gensandbox_drive_size() == 0) {
        print_traced();
        write_log("Sandbox traced by checking disk size <= 50GB");
        write_trace("hi_sandbox_drive_size");
    }
    else {
        print_not_traced();
    }
    
    /* Hooks detection tricks */
    printf("\n[-] Hooks detection\n");
    printf("[*] Checking function DeleteFileW method 1 ... ");
    if (check_hook_DeleteFileW_m1() == 0) {
        print_traced();
        write_log("Hooks traced using DeleteFileW method 1");
        write_trace("hi_hooks_deletefile_m1");
    }
    else {
        print_not_traced();
    }
    
    /* Sandboxie detection tricks */
    printf("\n[-] Sandboxie detection\n");
    printf("[*] Using sbiedll.dll ... ");
    if (sboxie_detect_sbiedll() == 0) {
        write_log("Sandboxie traced using sbiedll.dll");
        print_traced();
        write_trace("hi_sandboxie");
    }
    else {
        print_not_traced();
    }
    
    /* Wine detection tricks */
    printf("\n[-] Wine detection\n");
    printf("[*] Using GetProcAddress(wine_get_unix_file_name) from kernel32.dll ... ");
    if (wine_detect_get_unix_file_name() == 0) {
        write_log("Wine traced using GetProcAddress(wine_get_unix_file_name) from kernel32.dll");
        print_traced();
        write_trace("hi_wine");
    }
    else {
        print_not_traced();
    }
    
    /* VirtualBox detection tricks */
    printf("\n[-] VirtualBox detection\n");
    printf("[*] Scsi port->bus->target id->logical unit id-> 0 identifier ... ");
    if (vbox_reg_key1() == 0) {
        write_log("VirtualBox traced using Reg key HKLM\\HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0 \"Identifier\"");
        print_traced();
        write_trace("hi_virtualbox");
    }
    else {
        print_not_traced();
    }
    printf("[*] Reg key (HKLM\\HARDWARE\\Description\\System \"SystemBiosVersion\") ... ");
    if (vbox_reg_key2() == 0) {
        write_log("VirtualBox traced using Reg key HKLM\\HARDWARE\\Description\\System \"SystemBiosVersion\"");
        print_traced();
        write_trace("hi_virtualbox");
    }
    else {
        print_not_traced();
    }
    printf("[*] Reg key (HKLM\\SOFTWARE\\Oracle\\VirtualBox Guest Additions) ... ");
    if (vbox_reg_key3() == 0) {
        write_log("VirtualBox traced using Reg key HKLM\\SOFTWARE\\Oracle\\VirtualBox Guest Additions");
        print_traced();
        write_trace("hi_virtualbox");
    }
    else {
        print_not_traced();
    }
    printf("[*] Reg key (HKLM\\HARDWARE\\Description\\System \"VideoBiosVersion\") ... ");
    if (vbox_reg_key4() == 0) {
        write_log("VirtualBox traced using Reg key HKLM\\HARDWARE\\Description\\System \"VideoBiosVersion\"");
        print_traced();
        write_trace("hi_virtualbox");
    }
    else {
        print_not_traced();
    }

    printf("[*] Reg key (HKLM\\HARDWARE\\ACPI\\DSDT\\VBOX__ ... ");
    if (vbox_reg_key5() == 0) {
        write_log("VirtualBox traced using Reg key HKLM\\HARDWARE\\ACPI\\DSDT\\VBOX__");
        print_traced();
        write_trace("hi_virtualbox");
    }
    else {
        print_not_traced();
    }

    printf("[*] Reg key (HKLM\\SYSTEM\\CurrentControlSet\\Enum\\IDE ... ");
    if (vbox_reg_key6() == 0) {
    }
    else {
        print_not_traced();
    }

    printf("[*] Reg key (HKLM\\HARDWARE\\ACPI\\FADT\\VBOX__ ... ");
    if (vbox_reg_key7() == 0) {
        write_log("VirtualBox traced using Reg key HKLM\\HARDWARE\\ACPI\\FADT\\VBOX__");
        print_traced();
        write_trace("hi_virtualbox");
    }
    else {
        print_not_traced();
    }

    printf("[*] Reg key (HKLM\\HARDWARE\\ACPI\\RSDT\\VBOX__ ... ");
    if (vbox_reg_key8() == 0) {
        write_log("VirtualBox traced using Reg key HKLM\\HARDWARE\\ACPI\\RSDT\\VBOX__");
        print_traced();
        write_trace("hi_virtualbox");
    }
    else {
        print_not_traced();
    }

    printf("[*] Reg key (HKLM\\SYSTEM\\ControlSet001\\Services\\VBox* ... ");
    if (vbox_reg_key9() == 0) {        
    }
    else {
        print_not_traced();
    }


    if (vbox_sysfile1() == 0) {        
    }
    else {
        print_not_traced();
    }

    if (vbox_sysfile2() == 0) {
    }
    else {
        print_not_traced();
    }

    printf("[*] Looking for MAC ");
    if (vbox_mac() == 0) {
    }
    else {
        print_not_traced();
    }

    printf("[*] Looking for pseudo device ");
    if (vbox_pseudodev() == 0) {
    }
    else {
        print_not_traced();
    }

    printf("[*] Looking for pipe ");
    if (vbox_pipe() == 0) {
    }
    else {
        print_not_traced();
    }

    printf("[*] Looking for VBox tray tool window ");
    if (vbox_traywindow() == 0) {
    }
    else {
        print_not_traced();
    }

    printf("[*] Looking for VBox network share ");
    if (vbox_network_share() == 0) {
    }
    else {
        print_not_traced();
    }

    printf("[*] Looking for VBox processes ");
    if (vbox_processes() == 0) {
    }
    else {
        print_not_traced();
    }
    
    /* VMware detection tricks */
    printf("\n[-] VMware detection\n");
    printf("[*] Scsi port->bus->target id->logical unit id-> 0 identifier ... ");
    if (vmware_reg_key1() == 0) {
        write_log("VMWare traced using Reg key HKLM\\HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0 \"Identifier\"");
        print_traced();
        write_trace("hi_vmware");
    }
    else {
        print_not_traced();
    }
    printf("[*] Reg key (HKLM\\SOFTWARE\\VMware, Inc.\\VMware Tools) ... ");
    if (vmware_reg_key2() == 0) {
        write_log("VMware traced using Reg key HKLM\\SOFTWARE\\VMware, Inc.\\VMware Tools");
        print_traced();
        write_trace("hi_vmware");
    }
    else {
        print_not_traced();
    }
    printf("[*] Looking for C:\\WINDOWS\\system32\\drivers\\vmmouse.sys ... ");
    if (vmware_sysfile1() == 0) {
        write_log("VMware traced using file C:\\WINDOWS\\system32\\drivers\\vmmouse.sys");
        print_traced();
        write_trace("hi_vmware");
    }
    else {
        print_not_traced();
    }
    printf("[*] Looking for C:\\WINDOWS\\system32\\drivers\\vmhgfs.sys ... ");
    if (vmware_sysfile2() == 0) {
        write_log("VMware traced using file C:\\WINDOWS\\system32\\drivers\\vmhgfs.sys");
        print_traced();
        write_trace("hi_vmware");
    }
    else {
        print_not_traced();
    }
    
    /* Qemu detection tricks */
    printf("\n[-] Qemu detection\n");
    printf("[*] Scsi port->bus->target id->logical unit id-> 0 identifier ... ");
    if (qemu_reg_key1() == 0) {
        write_log("Qemu traced using Reg key HKLM\\HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0 \"Identifier\"");
        print_traced();
        write_trace("hi_qemu");
    }
    else {
        print_not_traced();
    }
    printf("[*] Reg key (HKLM\\HARDWARE\\Description\\System \"SystemBiosVersion\") ... ");
    if (qemu_reg_key2() == 0) {
        write_log("Qemu traced using Reg key HKLM\\HARDWARE\\Description\\System \"SystemBiosVersion\"");
        print_traced();
        write_trace("hi_qemu");
    }
    else {
        print_not_traced();
    }
    
    printf("\n\n");
    printf("[-] Finished, feel free to RE me.");
    
    write_log("End");
    
    fflush(stdin); getchar();
    return 0;
}
