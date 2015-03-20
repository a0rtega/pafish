
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

inline int pafish_exists_regkey_value_str(HKEY hKey, char * regkey_s, char * value_s, char * lookup) {
	HKEY regkey;
	LONG ret;
	DWORD size;
	char value[1024];

	size = sizeof(value);
	ret = RegOpenKeyEx(hKey, regkey_s, 0, KEY_READ, &regkey);
	if (ret == ERROR_SUCCESS) {
		ret = RegQueryValueEx(regkey, value_s, NULL, NULL, (BYTE*)value, &size);
		RegCloseKey(regkey);
		if (ret == ERROR_SUCCESS) {
			int i;
			for (i = 0; i < strlen(value); i++) { /* case-insensitive */
				value[i] = toupper(value[i]);
			}
			if (strstr(value, lookup) != NULL)
				return TRUE;
		}
	}
	return FALSE;
}

inline int pafish_exists_file(char * filename) {
	return (GetFileAttributes(filename) != INVALID_FILE_ATTRIBUTES) ? TRUE : FALSE;
}

