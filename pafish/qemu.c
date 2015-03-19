
#include <windows.h>
#include <string.h>

#include "qemu.h"
#include "types.h"

int qemu_reg_key1() {
	HKEY regkey;
	LONG retu;
	char value[1024];
	DWORD size;

	size = sizeof(value);
	retu = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", 0, KEY_READ, &regkey);
	if (retu == ERROR_SUCCESS) {
		retu = RegQueryValueEx(regkey, "Identifier", NULL, NULL, (BYTE*)value, &size);
		if (retu == ERROR_SUCCESS) {
			int i;
			for (i = 0; i < strlen(value); i++) { /* case-insensitive */
				value[i] = toupper(value[i]);
			}
			if (strstr(value, "QEMU") != NULL) {
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}
	}
	else {
		return FALSE;
	}
}

int qemu_reg_key2() {
	HKEY regkey;
	LONG retu;
	char value[1024];
	DWORD size;

	size = sizeof(value);
	retu = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\Description\\System", 0, KEY_READ, &regkey);
	if (retu == ERROR_SUCCESS) {
		retu = RegQueryValueEx(regkey, "SystemBiosVersion", NULL, NULL, (BYTE*)value, &size);
		if (retu == ERROR_SUCCESS) {
			int i;
			for (i = 0; i < strlen(value); i++) { /* case-insensitive */
				value[i] = toupper(value[i]);
			}
			if (strstr(value, "QEMU") != NULL) {
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}
	}
	else {
		return FALSE;
	}
}
