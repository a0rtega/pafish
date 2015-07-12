
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <wbemidl.h>

#include "types.h"
#include "common.h"

#include "debuggers.h"
#include "sandboxie.h"
#include "gensandbox.h"
#include "hooks.h"
#include "vbox.h"
#include "wine.h"
#include "vmware.h"
#include "qemu.h"
#include "cpu.h"
#include "cuckoo.h"

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

	Blue fish icon thanks to http://www.fasticon.com/

*/

int main(void)
{
	char winverstr[32], aux[1024];
	char cpu_vendor[13];
	OSVERSIONINFO winver;
	unsigned short original_colors = 0;

	write_log("Start");

	original_colors = init_cmd_colors();
	print_header();

	winver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&winver);
	snprintf(winverstr, sizeof(winverstr)-sizeof(winverstr[0]), "%lu.%lu build %lu",
	winver.dwMajorVersion, winver.dwMinorVersion, winver.dwBuildNumber);

	/* Get CPU vendor */
	cpu_write_vendor(cpu_vendor);

	printf("[*] Windows version: %s\n", winverstr);
	printf("[*] CPU vendor: %s\n", cpu_vendor);
	snprintf(aux, sizeof(aux)-sizeof(aux[0]), "Windows version: %s", winverstr);
	write_log(aux);
	snprintf(aux, sizeof(aux)-sizeof(aux[0]), "CPU vendor: %s", cpu_vendor);
	write_log(aux);

	/* Debuggers detection tricks */
	printf("\n[-] Debuggers detection\n");
	printf("[*] Using IsDebuggerPresent() ... ");
	if (debug_isdebuggerpresent() == TRUE) {
		write_log("Debugger traced using IsDebuggerPresent()");
		print_traced();
		write_trace("hi_debugger_isdebuggerpresent");
	}
	else print_not_traced();

	/* This is only working on MS Windows systems prior to Vista */
	if (winver.dwMajorVersion < 6) {
		printf("[*] Using OutputDebugString() ... ");
		if (debug_outputdebugstring() == TRUE) {
			write_log("Debugger traced using OutputDebugString()");
			print_traced();
			write_trace("hi_debugger_outputdebugstring");
		}
		else print_not_traced();
	}

	/* CPU information based detection tricks */
	printf("\n[-] CPU information based detections\n");
	printf("[*] Checking the difference between CPU timestamp counters (rdtsc) ... ");
	if (cpu_rdtsc() == TRUE) {
		print_traced();
		write_log("CPU VM traced by checking the difference between CPU timestamp counters (rdtsc)");
		write_trace("hi_CPU_VM_rdtsc");
	}
	else print_not_traced();

	printf("[*] Checking the difference between CPU timestamp counters (rdtsc) forcing VM exit ... ");
	if (cpu_rdtsc_force_vmexit() == TRUE) {
		print_traced();
		write_log("CPU VM traced by checking the difference between CPU timestamp counters (rdtsc) forcing VM exit");
		write_trace("hi_CPU_VM_rdtsc_force_vm_exit");
	}
	else print_not_traced();

	printf("[*] Checking hypervisor bit in cpuid feature bits ... ");
	if (cpu_hv() == TRUE) {
		print_traced();
		write_log("CPU VM traced by checking hypervisor bit in cpuid feature bits");
		write_trace("hi_CPU_VM_hypervisor_bit");
	}
	else print_not_traced();

	printf("[*] Checking cpuid vendor for known VM vendors ... ");
	if (cpu_known_vm_vendors(cpu_vendor) == TRUE) {
		print_traced();
		write_log("CPU VM traced by checking cpuid vendor for known VM vendors");
		write_trace("hi_CPU_VM_vendor_name");
	}
	else print_not_traced();

	/* Generic sandbox detection tricks */
	printf("\n[-] Generic sandbox detection\n");
	printf("[*] Using mouse activity ... ");
	if (gensandbox_mouse_act() == TRUE) {
		print_traced();
		write_log("Sandbox traced using mouse activity");
		write_trace("hi_sandbox_mouse_act");
	}
	else print_not_traced();

	printf("[*] Checking username ... ");
	if (gensandbox_username() == TRUE) {
		print_traced();
		write_log("Sandbox traced by checking username");
		write_trace("hi_sandbox_username");
	}
	else print_not_traced();

	printf("[*] Checking file path ... ");
	if (gensandbox_path() == TRUE) {
		print_traced();
		write_log("Sandbox traced by checking file path");
		write_trace("hi_sandbox_path");
	}
	else print_not_traced();

	printf("[*] Checking common sample names in drives root ... ");
	if (gensandbox_common_names() == TRUE) {
		print_traced();
		write_log("Sandbox traced by checking common sample names in drives root");
		write_trace("hi_sandbox_common_names");
	}
	else print_not_traced();

	printf("[*] Checking if disk size <= 60GB via DeviceIoControl() ... ");
	if (gensandbox_drive_size() == TRUE) {
		print_traced();
		write_log("Sandbox traced by checking disk size <= 60GB via DeviceIoControl()");
		write_trace("hi_sandbox_drive_size");
	}
	else print_not_traced();

	printf("[*] Checking if disk size <= 60GB via GetDiskFreeSpaceExA() ... ");
	if (gensandbox_drive_size2() == TRUE) {
		print_traced();
		write_log("Sandbox traced by checking disk size <= 60GB via GetDiskFreeSpaceExA()");
		write_trace("hi_sandbox_drive_size2");
	}
	else print_not_traced();

	printf("[*] Checking if Sleep() is patched using GetTickCount() ... ");
	if (gensandbox_sleep_patched() == TRUE) {
		print_traced();
		write_log("Sandbox traced by checking if Sleep() was patched using GetTickCount()");
		write_trace("hi_sandbox_sleep_gettickcount");
	}
	else print_not_traced();

	printf("[*] Checking if NumberOfProcessors is < 2 via raw access ... ");
	if (gensandbox_one_cpu() == TRUE) {
		print_traced();
		write_log("Sandbox traced by checking if NumberOfProcessors is less than 2 via raw access");
		write_trace("hi_sandbox_NumberOfProcessors_less_2_raw");
	}
	else print_not_traced();

	printf("[*] Checking if NumberOfProcessors is < 2 via GetSystemInfo() ... ");
	if (gensandbox_one_cpu_GetSystemInfo() == TRUE) {
		print_traced();
		write_log("Sandbox traced by checking if NumberOfProcessors is less than 2 via GetSystemInfo()");
		write_trace("hi_sandbox_NumberOfProcessors_less_2_GetSystemInfo");
	}
	else print_not_traced();

	printf("[*] Checking if pysical memory is < 1Gb ... ");
	if (gensandbox_less_than_onegb() == TRUE) {
		print_traced();
		write_log("Sandbox traced by checking if pysical memory is less than 1Gb");
		write_trace("hi_sandbox_pysicalmemory_less_1Gb");
	}
	else print_not_traced();

	/* Hooks detection tricks */
	printf("\n[-] Hooks detection\n");
	printf("[*] Checking function DeleteFileW method 1 ... ");
	if (check_hook_DeleteFileW_m1() == TRUE) {
		print_traced();
		write_log("Hooks traced using DeleteFileW method 1");
		write_trace("hi_hooks_deletefile_m1");
	}
	else print_not_traced();

	/* Sandboxie detection tricks */
	printf("\n[-] Sandboxie detection\n");
	printf("[*] Using GetModuleHandle(sbiedll.dll) ... ");
	if (sboxie_detect_sbiedll() == TRUE) {
		write_log("Sandboxie traced using GetModuleHandle(sbiedll.dll)");
		print_traced();
		write_trace("hi_sandboxie");
	}
	else print_not_traced();

	/* Wine detection tricks */
	printf("\n[-] Wine detection\n");
	printf("[*] Using GetProcAddress(wine_get_unix_file_name) from kernel32.dll ... ");
	if (wine_detect_get_unix_file_name() == TRUE) {
		write_log("Wine traced using GetProcAddress(wine_get_unix_file_name) from kernel32.dll");
		print_traced();
		write_trace("hi_wine");
	}
	else print_not_traced();

	printf("[*] Reg key (HKCU\\SOFTWARE\\Wine) ... ");
	if (wine_reg_key1() == TRUE) {
		write_log("Wine traced using Reg key HKCU\\SOFTWARE\\Wine");
		print_traced();
		write_trace("hi_wine");
	}
	else print_not_traced();

	/* VirtualBox detection tricks */
	printf("\n[-] VirtualBox detection\n");
	printf("[*] Scsi port->bus->target id->logical unit id-> 0 identifier ... ");
	if (vbox_reg_key1() == TRUE) {
		write_log("VirtualBox traced using Reg key HKLM\\HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0 \"Identifier\"");
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Reg key (HKLM\\HARDWARE\\Description\\System \"SystemBiosVersion\") ... ");
	if (vbox_reg_key2() == TRUE) {
		write_log("VirtualBox traced using Reg key HKLM\\HARDWARE\\Description\\System \"SystemBiosVersion\"");
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Reg key (HKLM\\SOFTWARE\\Oracle\\VirtualBox Guest Additions) ... ");
	if (vbox_reg_key3() == TRUE) {
		write_log("VirtualBox traced using Reg key HKLM\\SOFTWARE\\Oracle\\VirtualBox Guest Additions");
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Reg key (HKLM\\HARDWARE\\Description\\System \"VideoBiosVersion\") ... ");
	if (vbox_reg_key4() == TRUE) {
		write_log("VirtualBox traced using Reg key HKLM\\HARDWARE\\Description\\System \"VideoBiosVersion\"");
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Reg key (HKLM\\HARDWARE\\ACPI\\DSDT\\VBOX__ ... ");
	if (vbox_reg_key5() == TRUE) {
		write_log("VirtualBox traced using Reg key HKLM\\HARDWARE\\ACPI\\DSDT\\VBOX__");
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Reg key (HKLM\\HARDWARE\\ACPI\\FADT\\VBOX__ ... ");
	if (vbox_reg_key7() == TRUE) {
		write_log("VirtualBox traced using Reg key HKLM\\HARDWARE\\ACPI\\FADT\\VBOX__");
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Reg key (HKLM\\HARDWARE\\ACPI\\RSDT\\VBOX__ ... ");
	if (vbox_reg_key8() == TRUE) {
		write_log("VirtualBox traced using Reg key HKLM\\HARDWARE\\ACPI\\RSDT\\VBOX__");
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Reg key (HKLM\\SYSTEM\\ControlSet001\\Services\\VBox* ... ");
	if (vbox_reg_key9(TRUE) == TRUE) {
		/* Log written inside function */
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Reg key (HKLM\\HARDWARE\\DESCRIPTION\\System \"SystemBiosDate\" ... ");
	if (vbox_reg_key10() == TRUE) {
		write_log("VirtualBox traced using Reg key HKLM\\HARDWARE\\DESCRIPTION\\System \"SystemBiosDate\"");
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Driver files in C:\\WINDOWS\\system32\\drivers\\VBox* ... ");
	if (vbox_sysfile1(TRUE) == TRUE) {
		/* Log written inside function */
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Additional system files ... ");
	if (vbox_sysfile2(TRUE) == TRUE) {
		/* Log written inside function */
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Looking for a MAC address starting with 08:00:27 ... ");
	if (vbox_mac() == TRUE) {
		write_log("VirtualBox traced using MAC address starting with 08:00:27");
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Looking for pseudo devices ... ");
	if (vbox_devices(TRUE) == TRUE) {
		/* Log written inside function */
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Looking for VBoxTray windows ... ");
	if (vbox_traywindow() == TRUE) {
		write_log("VirtualBox traced using VBoxTray windows");
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Looking for VBox network share ... ");
	if (vbox_network_share() == TRUE) {
		write_log("VirtualBox traced using its network share");
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Looking for VBox processes (vboxservice.exe, vboxtray.exe) ... ");
	if (vbox_processes(TRUE) == TRUE) {
		/* Log written inside function */
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	printf("[*] Looking for VBox devices using WMI ... ");
	if (vbox_wmi_devices() == TRUE) {
		write_log("VirtualBox device identifiers traced using WMI");
		print_traced();
		write_trace("hi_virtualbox");
	}
	else print_not_traced();

	/* VMware detection tricks */
	printf("\n[-] VMware detection\n");
	printf("[*] Scsi port 0,1,2 ->bus->target id->logical unit id-> 0 identifier ... ");
	if (vmware_reg_key1() == TRUE) {
		write_log("VMWare traced using Reg key HKLM\\HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0,1,2\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0 \"Identifier\"");
		print_traced();
		write_trace("hi_vmware");
	}
	else print_not_traced();

	printf("[*] Reg key (HKLM\\SOFTWARE\\VMware, Inc.\\VMware Tools) ... ");
	if (vmware_reg_key2() == TRUE) {
		write_log("VMware traced using Reg key HKLM\\SOFTWARE\\VMware, Inc.\\VMware Tools");
		print_traced();
		write_trace("hi_vmware");
	}
	else print_not_traced();

	printf("[*] Looking for C:\\WINDOWS\\system32\\drivers\\vmmouse.sys ... ");
	if (vmware_sysfile1() == TRUE) {
		write_log("VMware traced using file C:\\WINDOWS\\system32\\drivers\\vmmouse.sys");
		print_traced();
		write_trace("hi_vmware");
	}
	else print_not_traced();

	printf("[*] Looking for C:\\WINDOWS\\system32\\drivers\\vmhgfs.sys ... ");
	if (vmware_sysfile2() == TRUE) {
		write_log("VMware traced using file C:\\WINDOWS\\system32\\drivers\\vmhgfs.sys");
		print_traced();
		write_trace("hi_vmware");
	}
	else print_not_traced();

	printf("[*] Looking for a MAC address starting with 00:05:69, 00:0C:29, 00:1C:14 or 00:50:56 ... ");
	if (vmware_mac() == TRUE) {
		write_log("VMware traced using MAC address starting with 00:05:69, 00:0C:29, 00:1C:14 or 00:50:56");
		print_traced();
		write_trace("hi_vmware");
	}
	else print_not_traced();

	printf("[*] Looking for pseudo devices ... ");
	if (vmware_devices(TRUE) == TRUE) {
		/* Log written inside function */
		print_traced();
		write_trace("hi_vmware");
	}
	else print_not_traced();

	printf("[*] Looking for VMware serial number ... ");
	if (vmware_wmi_serial() == TRUE) {
		write_log("VMware serial number traced using WMI");
		print_traced();
		write_trace("hi_vmware");
	}
	else print_not_traced();

	/* Qemu detection tricks */
	printf("\n[-] Qemu detection\n");
	printf("[*] Scsi port->bus->target id->logical unit id-> 0 identifier ... ");
	if (qemu_reg_key1() == TRUE) {
		write_log("Qemu traced using Reg key HKLM\\HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0 \"Identifier\"");
		print_traced();
		write_trace("hi_qemu");
	}
	else print_not_traced();

	printf("[*] Reg key (HKLM\\HARDWARE\\Description\\System \"SystemBiosVersion\") ... ");
	if (qemu_reg_key2() == TRUE) {
		write_log("Qemu traced using Reg key HKLM\\HARDWARE\\Description\\System \"SystemBiosVersion\"");
		print_traced();
		write_trace("hi_qemu");
	}
	else print_not_traced();

	/* Cuckoo detection tricks */
	printf("\n[-] Cuckoo detection\n");
	printf("[*] Looking in the TLS for the hooks information structure ... ");
	if (cuckoo_check_tls() == TRUE) {
		write_log("Cuckoo hooks information structure traced in the TLS");
		print_traced();
		write_trace("hi_cuckoo");
	}
	else print_not_traced();

	printf("\n\n");
	printf("[-] Feel free to RE me, check log file for more information.");

	write_log("End");

	getchar();

	/* Restore Original Console Colors */
	restore_cmd_colors(original_colors);

	return 0;
}
