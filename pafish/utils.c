
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>

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
	char value[1024], * lookup_str;
	size_t lookup_size;

	lookup_size = strlen(lookup);
	lookup_str = malloc(lookup_size+sizeof(char));
	strncpy(lookup_str, lookup, lookup_size+sizeof(char));

	size = sizeof(value);
	ret = RegOpenKeyEx(hKey, regkey_s, 0, KEY_READ, &regkey);
	if (ret == ERROR_SUCCESS) {
		ret = RegQueryValueEx(regkey, value_s, NULL, NULL, (BYTE*)value, &size);
		RegCloseKey(regkey);
		if (ret == ERROR_SUCCESS) {
			size_t i;
			for (i = 0; i < strlen(value); i++) { /* case-insensitive */
				value[i] = toupper(value[i]);
			}
			for (i = 0; i < lookup_size; i++) { /* case-insensitive */
				lookup_str[i] = toupper(lookup_str[i]);
			}
			if (strstr(value, lookup_str) != NULL) {
				free(lookup_str);
				return TRUE;
			}
		}
	}
	free(lookup_str);
	return FALSE;
}

inline int pafish_exists_file(char * filename) {
	return (GetFileAttributes(filename) != INVALID_FILE_ATTRIBUTES) ? TRUE : FALSE;
}

