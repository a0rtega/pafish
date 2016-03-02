
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "config.h"
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
#include "bochs.h"

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
	char cpu_vendor[13], cpu_hv_vendor[13], cpu_brand[49];
	OSVERSIONINFO winver;
	unsigned short original_colors = 0;

	write_log("Start");
	#if ENABLE_DNS_TRACE
		write_trace_dns("analysis-start");
	#endif

	original_colors = init_cmd_colors();
	print_header();

	winver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&winver);
	snprintf(winverstr, sizeof(winverstr) - sizeof(winverstr[0]),
		 "%lu.%lu build %lu", winver.dwMajorVersion,
		 winver.dwMinorVersion, winver.dwBuildNumber);

	/* Get CPU vendor */
	cpu_write_vendor(cpu_vendor);
	cpu_write_hv_vendor(cpu_hv_vendor);
	cpu_write_brand(cpu_brand);

	printf("[*] Windows version: %s\n", winverstr);
	printf("[*] CPU: %s\n", cpu_vendor);
	if (strlen(cpu_hv_vendor))
		printf("    Hypervisor: %s\n", cpu_hv_vendor);
	printf("    CPU brand: %s\n", cpu_brand);
	snprintf(aux, sizeof(aux) - sizeof(aux[0]), "Windows version: %s",
		 winverstr);
	write_log(aux);
	if (strlen(cpu_hv_vendor))
		snprintf(aux, sizeof(aux) - sizeof(aux[0]), "CPU: %s (HV: %s) %s", cpu_vendor,
			cpu_hv_vendor, cpu_brand);
	else
		snprintf(aux, sizeof(aux) - sizeof(aux[0]), "CPU: %s %s", cpu_vendor,
			cpu_brand);
	write_log(aux);

	/* Debuggers detection tricks */
	print_check_group("Debuggers detection");
	exec_check("Using IsDebuggerPresent()", &debug_isdebuggerpresent,
		   "Debugger traced using IsDebuggerPresent()",
		   "hi_debugger_isdebuggerpresent");
	/* This is only working on MS Windows systems prior to Vista */
	if (winver.dwMajorVersion < 6) {
		exec_check("Using OutputDebugString()",
			   &debug_outputdebugstring,
			   "Debugger traced using OutputDebugString()",
			   "hi_debugger_outputdebugstring");
	}

	/* CPU information based detection tricks */
	print_check_group("CPU information based detections");
	exec_check
	    ("Checking the difference between CPU timestamp counters (rdtsc)",
	     &cpu_rdtsc,
	     "CPU VM traced by checking the difference between CPU timestamp counters (rdtsc)",
	     "hi_CPU_VM_rdtsc");
	exec_check
	    ("Checking the difference between CPU timestamp counters (rdtsc) forcing VM exit",
	     &cpu_rdtsc_force_vmexit,
	     "CPU VM traced by checking the difference between CPU timestamp counters (rdtsc) forcing VM exit",
	     "hi_CPU_VM_rdtsc_force_vm_exit");
	exec_check("Checking hypervisor bit in cpuid feature bits", &cpu_hv,
		   "CPU VM traced by checking hypervisor bit in cpuid feature bits",
		   "hi_CPU_VM_hypervisor_bit");
	exec_check("Checking cpuid hypervisor vendor for known VM vendors",
		   &cpu_known_vm_vendors,
		   "CPU VM traced by checking cpuid hypervisor vendor for known VM vendors",
		   "hi_CPU_VM_hv_vendor_name");

	/* Generic sandbox detection tricks */
	print_check_group("Generic sandbox detection");
	exec_check("Using mouse activity", &gensandbox_mouse_act,
		   "Sandbox traced using mouse activity",
		   "hi_sandbox_mouse_act");
	exec_check("Checking username", &gensandbox_username,
		   "Sandbox traced by checking username",
		   "hi_sandbox_username");
	exec_check("Checking file path", &gensandbox_path,
		   "Sandbox traced by checking file path", "hi_sandbox_path");
	exec_check("Checking common sample names in drives root",
		   &gensandbox_common_names,
		   "Sandbox traced by checking common sample names in drives root",
		   "hi_sandbox_common_names");
	exec_check("Checking if disk size <= 60GB via DeviceIoControl()",
		   &gensandbox_drive_size,
		   "Sandbox traced by checking disk size <= 60GB via DeviceIoControl()",
		   "hi_sandbox_drive_size");
	exec_check("Checking if disk size <= 60GB via GetDiskFreeSpaceExA()",
		   &gensandbox_drive_size2,
		   "Sandbox traced by checking disk size <= 60GB via GetDiskFreeSpaceExA()",
		   "hi_sandbox_drive_size2");
	exec_check("Checking if Sleep() is patched using GetTickCount()",
		   &gensandbox_sleep_patched,
		   "Sandbox traced by checking if Sleep() was patched using GetTickCount()",
		   "hi_sandbox_sleep_gettickcount");
	exec_check("Checking if NumberOfProcessors is < 2 via raw access",
		   &gensandbox_one_cpu,
		   "Sandbox traced by checking if NumberOfProcessors is less than 2 via raw access",
		   "hi_sandbox_NumberOfProcessors_less_2_raw");
	exec_check("Checking if NumberOfProcessors is < 2 via GetSystemInfo()",
		   &gensandbox_one_cpu_GetSystemInfo,
		   "Sandbox traced by checking if NumberOfProcessors is less than 2 via GetSystemInfo()",
		   "hi_sandbox_NumberOfProcessors_less_2_GetSystemInfo");
	exec_check("Checking if pysical memory is < 1Gb",
		   &gensandbox_less_than_onegb,
		   "Sandbox traced by checking if pysical memory is less than 1Gb",
		   "hi_sandbox_pysicalmemory_less_1Gb");
	exec_check("Checking operating system uptime using GetTickCount()",
		   &gensandbox_uptime,
		   "Sandbox traced by checking operating system uptime using GetTickCount()",
		   "hi_sandbox_uptime");
	exec_check("Checking if operating system IsNativeVhdBoot()",
		   &gensandbox_IsNativeVhdBoot,
		   "Sandbox traced by checking IsNativeVhdBoot()",
		   "hi_sandbox_IsNativeVhdBoot");

	/* Hooks detection tricks */
	print_check_group("Hooks detection");
	exec_check("Checking function ShellExecuteExW method 1",
		   &check_hook_ShellExecuteExW_m1,
		   "Hooks traced using ShellExecuteExW method 1",
		   "hi_hooks_shellexecuteexw_m1");
	exec_check("Checking function CreateProcessA method 1",
		   &check_hook_CreateProcessA_m1,
		   "Hooks traced using CreateProcessA method 1",
		   "hi_hooks_createprocessa_m1");

	/* Sandboxie detection tricks */
	print_check_group("Sandboxie detection");
	exec_check("Using GetModuleHandle(sbiedll.dll)", &sboxie_detect_sbiedll,
		   "Sandboxie traced using GetModuleHandle(sbiedll.dll)",
		   "hi_sandboxie");

	/* Wine detection tricks */
	print_check_group("Wine detection");
	exec_check
	    ("Using GetProcAddress(wine_get_unix_file_name) from kernel32.dll",
	     &wine_detect_get_unix_file_name,
	     "Wine traced using GetProcAddress(wine_get_unix_file_name) from kernel32.dll",
	     "hi_wine");
	exec_check("Reg key (HKCU\\SOFTWARE\\Wine)", &wine_reg_key1,
		   "Wine traced using Reg key HKCU\\SOFTWARE\\Wine", "hi_wine");

	/* VirtualBox detection tricks */
	print_check_group("VirtualBox detection");
	exec_check("Scsi port->bus->target id->logical unit id-> 0 identifier",
		   &vbox_reg_key1,
		   "VirtualBox traced using Reg key HKLM\\HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0 \"Identifier\"",
		   "hi_virtualbox");
	exec_check
	    ("Reg key (HKLM\\HARDWARE\\Description\\System \"SystemBiosVersion\")",
	     &vbox_reg_key2,
	     "VirtualBox traced using Reg key HKLM\\HARDWARE\\Description\\System \"SystemBiosVersion\"",
	     "hi_virtualbox");
	exec_check
	    ("Reg key (HKLM\\SOFTWARE\\Oracle\\VirtualBox Guest Additions)",
	     &vbox_reg_key3,
	     "VirtualBox traced using Reg key HKLM\\SOFTWARE\\Oracle\\VirtualBox Guest Additions",
	     "hi_virtualbox");
	exec_check
	    ("Reg key (HKLM\\HARDWARE\\Description\\System \"VideoBiosVersion\")",
	     &vbox_reg_key4,
	     "VirtualBox traced using Reg key HKLM\\HARDWARE\\Description\\System \"VideoBiosVersion\"",
	     "hi_virtualbox");
	exec_check("Reg key (HKLM\\HARDWARE\\ACPI\\DSDT\\VBOX__)",
		   &vbox_reg_key5,
		   "VirtualBox traced using Reg key HKLM\\HARDWARE\\ACPI\\DSDT\\VBOX__",
		   "hi_virtualbox");
	exec_check("Reg key (HKLM\\HARDWARE\\ACPI\\FADT\\VBOX__)",
		   &vbox_reg_key7,
		   "VirtualBox traced using Reg key HKLM\\HARDWARE\\ACPI\\FADT\\VBOX__",
		   "hi_virtualbox");
	exec_check("Reg key (HKLM\\HARDWARE\\ACPI\\RSDT\\VBOX__)",
		   &vbox_reg_key8,
		   "VirtualBox traced using Reg key HKLM\\HARDWARE\\ACPI\\RSDT\\VBOX__",
		   "hi_virtualbox");
	exec_check("Reg key (HKLM\\SYSTEM\\ControlSet001\\Services\\VBox*)",
		   &vbox_reg_key9, NULL, "hi_virtualbox");
	exec_check
	    ("Reg key (HKLM\\HARDWARE\\DESCRIPTION\\System \"SystemBiosDate\")",
	     &vbox_reg_key10,
	     "VirtualBox traced using Reg key HKLM\\HARDWARE\\DESCRIPTION\\System \"SystemBiosDate\"",
	     "hi_virtualbox");
	exec_check("Driver files in C:\\WINDOWS\\system32\\drivers\\VBox*",
		   &vbox_sysfile1, NULL, "hi_virtualbox");
	exec_check("Additional system files", &vbox_sysfile2, NULL,
		   "hi_virtualbox");
	exec_check("Looking for a MAC address starting with 08:00:27",
		   &vbox_mac,
		   "VirtualBox traced using MAC address starting with 08:00:27",
		   "hi_virtualbox");
	exec_check("Looking for pseudo devices", &vbox_devices, NULL,
		   "hi_virtualbox");
	exec_check("Looking for VBoxTray windows", &vbox_traywindow,
		   "VirtualBox traced using VBoxTray windows", "hi_virtualbox");
	exec_check("Looking for VBox network share", &vbox_network_share,
		   "VirtualBox traced using its network share",
		   "hi_virtualbox");
	exec_check("Looking for VBox processes (vboxservice.exe, vboxtray.exe)",
		   &vbox_processes, NULL, "hi_virtualbox");
	exec_check("Looking for VBox devices using WMI", &vbox_wmi_devices,
		   "VirtualBox device identifiers traced using WMI",
		   "hi_virtualbox");

	/* VMware detection tricks */
	print_check_group("VMware detection");
	exec_check
	    ("Scsi port 0,1,2 ->bus->target id->logical unit id-> 0 identifier",
	     &vmware_reg_key1,
	     "VMWare traced using Reg key HKLM\\HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0,1,2\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0 \"Identifier\"",
	     "hi_vmware");
	exec_check("Reg key (HKLM\\SOFTWARE\\VMware, Inc.\\VMware Tools)",
		   &vmware_reg_key2,
		   "VMware traced using Reg key HKLM\\SOFTWARE\\VMware, Inc.\\VMware Tools",
		   "hi_vmware");
	exec_check("Looking for C:\\WINDOWS\\system32\\drivers\\vmmouse.sys",
		   &vmware_sysfile1,
		   "VMware traced using file C:\\WINDOWS\\system32\\drivers\\vmmouse.sys",
		   "hi_vmware");
	exec_check("Looking for C:\\WINDOWS\\system32\\drivers\\vmhgfs.sys",
		   &vmware_sysfile2,
		   "VMware traced using file C:\\WINDOWS\\system32\\drivers\\vmhgfs.sys",
		   "hi_vmware");
	exec_check
	    ("Looking for a MAC address starting with 00:05:69, 00:0C:29, 00:1C:14 or 00:50:56",
	     &vmware_mac,
	     "VMware traced using MAC address starting with 00:05:69, 00:0C:29, 00:1C:14 or 00:50:56",
	     "hi_vmware");
	exec_check("Looking for network adapter name", &vmware_adapter_name,
		   "VMware traced using network adapter name", "hi_vmware");
	exec_check("Looking for pseudo devices", &vmware_devices, NULL,
		   "hi_vmware");
	exec_check("Looking for VMware serial number", &vmware_wmi_serial,
		   "VMware serial number traced using WMI", "hi_vmware");

	/* Qemu detection tricks */
	print_check_group("Qemu detection");
	exec_check("Scsi port->bus->target id->logical unit id-> 0 identifier",
		   &qemu_reg_key1,
		   "Qemu traced using Reg key HKLM\\HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0 \"Identifier\"",
		   "hi_qemu");
	exec_check
	    ("Reg key (HKLM\\HARDWARE\\Description\\System \"SystemBiosVersion\")",
	     &qemu_reg_key2,
	     "Qemu traced using Reg key HKLM\\HARDWARE\\Description\\System \"SystemBiosVersion\"",
	     "hi_qemu");
	exec_check("cpuid CPU brand string 'QEMU Virtual CPU'", &qemu_cpu_name,
		   "Qemu traced using CPU brand string 'QEMU Virtual CPU'",
		   "hi_qemu");

	/* Bochs detection tricks */
	print_check_group("Bochs detection");
	exec_check
	    ("Reg key (HKLM\\HARDWARE\\Description\\System \"SystemBiosVersion\")",
	     &bochs_reg_key1,
	     "Bochs traced using Reg key HKLM\\HARDWARE\\Description\\System \"SystemBiosVersion\"",
	     "hi_bochs");
	exec_check("cpuid AMD wrong value for processor name", &bochs_cpu_amd1,
		   "Bochs traced using CPU AMD wrong value for processor name",
		   "hi_bochs");
	/*
	exec_check("cpuid AMD wrong value for Easter egg", &bochs_cpu_amd2,
		   "Bochs traced using CPU AMD wrong value for Easter egg",
		   "hi_bochs");
	*/
	exec_check("cpuid Intel wrong value for processor name",
		   &bochs_cpu_intel1,
		   "Bochs traced using CPU Intel wrong value for processor name",
		   "hi_bochs");

	/* Cuckoo detection tricks */
	print_check_group("Cuckoo detection");
	exec_check("Looking in the TLS for the hooks information structure",
		   &cuckoo_check_tls,
		   "Cuckoo hooks information structure traced in the TLS",
		   "hi_cuckoo");

	printf("\n\n");
	printf("[-] Feel free to RE me, check log file for more information.");

	write_log("End");
	#if ENABLE_DNS_TRACE
		write_trace_dns("analysis-end");
	#endif

	getchar();

	/* Restore Original Console Colors */
	restore_cmd_colors(original_colors);

	return 0;
}
