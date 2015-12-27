
#ifndef UTILS_H
#define UTILS_H

#include <wbemidl.h>

int pafish_disable_wow64_fs_redirection(void * old);

int pafish_revert_wow64_fs_redirection(void * old);

int pafish_iswow64();

int pafish_exists_regkey(HKEY hKey, char * regkey);

int pafish_exists_regkey_value_str(HKEY, char *, char *, char *);

int pafish_exists_file(char * filename);

int pafish_check_mac_vendor(char * mac_vendor);

int pafish_check_adapter_name(char * name);

/**
 * Prototype for the WMI caller implemented function for checking the
 * WMI query results.
 */
typedef int (*wmi_check_row) (IWbemClassObject *);

int wmi_initialize(const wchar_t *, IWbemServices **);

int wmi_check_query(IWbemServices *, const wchar_t *, const wchar_t *,
	wmi_check_row check_row);

void wmi_cleanup(IWbemServices *);

#endif
