
#include <windows.h>
#include <string.h>

#include "vmware.h"
#include "types.h"
#include "utils.h"

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
