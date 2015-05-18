
#include <windows.h>

#include "wine.h"
#include "types.h"
#include "utils.h"

int wine_detect_get_unix_file_name() {
	HMODULE k32;
	k32 = GetModuleHandle("kernel32.dll");
	if (k32 != NULL) {
		if (GetProcAddress(k32, "wine_get_unix_file_name") != NULL) {
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

int wine_reg_key1() {
	return pafish_exists_regkey(HKEY_CURRENT_USER, "SOFTWARE\\Wine");
}
