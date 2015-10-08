
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <wbemidl.h>

#include "vmware.h"
#include "types.h"
#include "utils.h"
#include "common.h"

int vmware_reg_key1() {
	if ( pafish_exists_regkey_value_str(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", "Identifier", "VMWARE") ||
	     pafish_exists_regkey_value_str(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 1\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", "Identifier", "VMWARE") ||
	     pafish_exists_regkey_value_str(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 2\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", "Identifier", "VMWARE")
	   )
		return TRUE;
	else
		return FALSE;
}

int vmware_reg_key2() {
	return pafish_exists_regkey(HKEY_LOCAL_MACHINE, "SOFTWARE\\VMware, Inc.\\VMware Tools");
}

int vmware_sysfile1() {
	return pafish_exists_file("C:\\WINDOWS\\system32\\drivers\\vmmouse.sys");
}

int vmware_sysfile2() {
	return pafish_exists_file("C:\\WINDOWS\\system32\\drivers\\vmhgfs.sys");
}

int vmware_mac() {
	/*
		VMware is any of
		00:05:69
		00:0C:29
		00:1C:14
		00:50:56
	*/
	if (pafish_check_mac_vendor("\x00\x05\x69")) {
		return TRUE;
	}
	else if (pafish_check_mac_vendor("\x00\x0C\x29")) {
		return TRUE;
	}
	else if (pafish_check_mac_vendor("\x00\x1C\x14")) {
		return TRUE;
	}
	else if (pafish_check_mac_vendor("\x00\x50\x56")) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

int vmware_adapter_name() {
	return pafish_check_adapter_name("VMware");
}

int vmware_devices(int writelogs) {
	HANDLE h;
	const int count = 2;
	string strs[count];
	int res = FALSE, i = 0;
	char message[200];

	strs[0] = "\\\\.\\HGFS";
	strs[1] = "\\\\.\\vmci";
	for (i=0; i < count; i++) {
		h = CreateFile(strs[i], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (h != INVALID_HANDLE_VALUE) {
			snprintf(message, sizeof(message)-sizeof(message[0]), "VMWare traced using device %s", strs[i]);
			if (writelogs) write_log(message);
			res = TRUE;
		}
	}
	return res;
}

/**
 * Check the serial number ("VMware") in the returned rows.
 */
int vmware_wmi_check_row(IWbemClassObject *row) {
	CIMTYPE type = CIM_ILLEGAL;
	VARIANT value;

	HRESULT hresult = row->lpVtbl->Get(row, L"SerialNumber", 0, &value, &type, 0);

	if (FAILED(hresult) || V_VT(&value) == VT_NULL || type != CIM_STRING) {
		return FALSE;
	}

	return (wcsstr(V_BSTR(&value), L"VMware") != NULL) ? TRUE : FALSE;
}

/**
 * Check for the computer serial using WMI.
 */
int vmware_wmi_serial() {
	IWbemServices *services = NULL;

	if (wmi_initialize(L"root\\cimv2", &services) != TRUE) {
		return FALSE;
	}

	int result = wmi_check_query(services, L"WQL", L"SELECT SerialNumber FROM Win32_Bios",
			&vmware_wmi_check_row);

	wmi_cleanup(services);

	return result;
}
