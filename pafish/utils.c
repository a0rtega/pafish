
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>

#include "utils.h"
#include "types.h"

/**
 * Prototypes for the Wow64 API's since they aren't available in all Windows
 * versions, most notably Windows XP 32 bits.
 */
typedef int (WINAPI *DisableWow64FsRedirectionProto) (void*);

typedef int (WINAPI *RevertWow64FsRedirectionProto) (void*);

typedef int (WINAPI *IsWow64ProcessProto) (HANDLE, int*);

/**
 * Wrapper function for Wow64DisableWow64FsRedirection. The function returns
 * FALSE if the Wow64DisableWow64FsRedirection is not found or the invocation
 * fails. The old value is returned in the argument old.
 */
int pafish_disable_wow64_fs_redirection(void * old) {
	DisableWow64FsRedirectionProto fndisable = (DisableWow64FsRedirectionProto) GetProcAddress(
		GetModuleHandleA("kernel32"), "Wow64DisableWow64FsRedirection");

	return (fndisable != NULL) && (fndisable(old) != 0) ? TRUE : FALSE;
}
/**
 * Wrapper function for Wow64RevertWow64FsRedirection. The function returns
 * FALSE if the Wow64RevertWow64FsRedirection is not found or the invocation
 * fails. The old value is to be provided using the argument old.
 */
int pafish_revert_wow64_fs_redirection(void * old) {
	RevertWow64FsRedirectionProto fnrevert = (RevertWow64FsRedirectionProto) GetProcAddress(
		GetModuleHandleA("kernel32"), "Wow64RevertWow64FsRedirection");

	return (fnrevert != NULL) && (fnrevert(old) != 0) ? TRUE : FALSE;
}

/**
 * Wrapper function for IsWow64Process. The function returns TRUE if the
 * current process is running under Wow64, FALSE otherwise or if the
 * invocation failed.
 */
int pafish_iswow64() {
	int result = FALSE;

	IsWow64ProcessProto fniswow = (IsWow64ProcessProto) GetProcAddress(
		GetModuleHandleA("kernel32"), "IsWow64Process");

	return (fniswow != NULL) && (fniswow(GetCurrentProcess(), &result) != 0)  ? result : FALSE;
}

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
	DWORD res = INVALID_FILE_ATTRIBUTES;

	if (pafish_iswow64() == TRUE) {
		void *old = NULL;

		//  Disable redirection immediately prior to calling GetFileAttributes.
		if (pafish_disable_wow64_fs_redirection(&old) ) {
			res = GetFileAttributes(filename);

			// Ignoring MSDN recommendation of exiting if this call fails.
			pafish_revert_wow64_fs_redirection(old);
		}
	}
	else {
		res = GetFileAttributes(filename);
	}

	return (res != INVALID_FILE_ATTRIBUTES) ? TRUE : FALSE;
}

