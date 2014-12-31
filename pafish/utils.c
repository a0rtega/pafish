
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "utils.h"
#include "types.h"

inline int pafish_exists_regkey(HKEY hKey, char * regkey_s) {
	HKEY regkey;
	LONG ret;

	ret = RegOpenKeyEx(hKey, regkey_s, 0, KEY_READ, &regkey);
	if (ret == ERROR_SUCCESS) {
		RegCloseKey(regkey);
		return TRUE;
	}
	else
	return FALSE;
}

inline int pafish_exists_file(char * filename) {
	DWORD ret;

	ret = GetFileAttributes(filename);
	if (ret != INVALID_FILE_ATTRIBUTES)
	return TRUE;
	else
	return FALSE;
}
