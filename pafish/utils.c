
#define _WIN32_WINNT 0x0501 /* _WIN32_WINNT_WINXP */

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <ctype.h>
#include <iphlpapi.h>
#include <tlhelp32.h>
#include <wbemidl.h>

#include "utils.h"
#include "types.h"

#ifndef KEY_WOW64_32KEY
#define KEY_WOW64_32KEY 0x0200
#endif

#ifndef KEY_WOW64_64KEY
#define KEY_WOW64_64KEY 0x0100
#endif

/**
 * Prototypes for the Wow64 API's since they aren't available in all Windows
 * versions, most notably Windows XP 32 bits.
 */
typedef BOOL (WINAPI *DisableWow64FsRedirectionProto) (void*);

typedef BOOL (WINAPI *RevertWow64FsRedirectionProto) (void*);

typedef BOOL (WINAPI *IsWow64ProcessProto) (HANDLE, BOOL*);

/**
 * Wrapper function for Wow64DisableWow64FsRedirection. The function returns
 * FALSE if the Wow64DisableWow64FsRedirection is not found or the invocation
 * fails. The old value is returned in the argument old.
 */
int pafish_disable_wow64_fs_redirection(void * old) {
	DisableWow64FsRedirectionProto fndisable = (DisableWow64FsRedirectionProto) GetProcAddress(
		GetModuleHandleA("kernel32"), "Wow64DisableWow64FsRedirection");

	return (fndisable) && (fndisable(old) != 0) ? TRUE : FALSE;
}

/**
 * Wrapper function for Wow64RevertWow64FsRedirection. The function returns
 * FALSE if the Wow64RevertWow64FsRedirection is not found or the invocation
 * fails. The old value is to be provided using the argument old.
 */
int pafish_revert_wow64_fs_redirection(void * old) {
	RevertWow64FsRedirectionProto fnrevert = (RevertWow64FsRedirectionProto) GetProcAddress(
		GetModuleHandleA("kernel32"), "Wow64RevertWow64FsRedirection");

	return (fnrevert) && (fnrevert(old) != 0) ? TRUE : FALSE;
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

	return (fniswow) && (fniswow(GetCurrentProcess(), &result) != 0) ? result : FALSE;
}

int pafish_exists_regkey(HKEY hKey, char * regkey_s) {
	HKEY regkey;
	LONG ret;

	if (pafish_iswow64()) {
		ret = RegOpenKeyEx(hKey, regkey_s, 0, KEY_READ | KEY_WOW64_64KEY, &regkey);
	}
	else {
		ret = RegOpenKeyEx(hKey, regkey_s, 0, KEY_READ, &regkey);
	}
	if (ret == ERROR_SUCCESS) {
		RegCloseKey(regkey);
		return TRUE;
	}
	else
		return FALSE;
}

int pafish_exists_regkey_value_str(HKEY hKey, char * regkey_s, char * value_s, char * lookup) {
	HKEY regkey;
	LONG ret;
	DWORD size;
	char value[1024], * lookup_str;
	size_t lookup_size;

	lookup_size = strlen(lookup);
	lookup_str = malloc(lookup_size+sizeof(char));
	strncpy(lookup_str, lookup, lookup_size+sizeof(char));

	size = sizeof(value);
	if (pafish_iswow64()) {
		ret = RegOpenKeyEx(hKey, regkey_s, 0, KEY_READ | KEY_WOW64_64KEY, &regkey);
	}
	else {
		ret = RegOpenKeyEx(hKey, regkey_s, 0, KEY_READ, &regkey);
	}
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

int pafish_exists_file(char * filename) {
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

int pafish_check_mac_vendor(char * mac_vendor) {
	unsigned long alist_size = 0, ret;

	ret = GetAdaptersAddresses(AF_UNSPEC,0,0,0,&alist_size);
	if(ret==ERROR_BUFFER_OVERFLOW) {
		IP_ADAPTER_ADDRESSES* palist = (IP_ADAPTER_ADDRESSES*)LocalAlloc(LMEM_ZEROINIT,alist_size);
		void * palist_free = palist;
		if(palist) {
			GetAdaptersAddresses(AF_UNSPEC,0,0,palist,&alist_size);
			char mac[6]={0};
			while (palist){
				if (palist->PhysicalAddressLength==0x6){
					memcpy(mac,palist->PhysicalAddress,0x6);
					if (!memcmp(mac_vendor, mac, 3)) { /* First 3 bytes are the same */
						LocalFree(palist_free);
						return TRUE;
					}
				}
				palist = palist->Next;
			}
			LocalFree(palist_free);
		}
	}
	return FALSE;
}

int pafish_check_adapter_name(char * name) {
	unsigned long alist_size = 0, ret;
	wchar_t aux[1024];

	mbstowcs(aux, name, sizeof(aux)-sizeof(aux[0]));

	ret = GetAdaptersAddresses(AF_UNSPEC, 0, 0, 0, &alist_size);
	if (ret == ERROR_BUFFER_OVERFLOW) {
		IP_ADAPTER_ADDRESSES *palist = (IP_ADAPTER_ADDRESSES*)LocalAlloc(LMEM_ZEROINIT, alist_size);
		void * palist_free = palist;
		if (palist) {
			if (GetAdaptersAddresses(AF_UNSPEC, 0, 0, palist, &alist_size) == ERROR_SUCCESS) {
				while (palist) {
					if (wcsstr(palist->Description, aux)) {
						LocalFree(palist_free);
						return TRUE;
					}
					palist = palist->Next;
				}
			}
			LocalFree(palist_free);
		}
	}
	return FALSE;
}

/**
 * Initialise the WMI client that will connect to the local machine WMI
 * namespace. It will return TRUE if the connection was successful, FALSE
 * otherwise.
 */
int wmi_initialize(const wchar_t *query_namespace, IWbemServices **services) {
	BSTR namespace;
	IWbemLocator *locator = NULL;
	int result;

	HRESULT hresult = CoInitializeEx(0, COINIT_MULTITHREADED);

	if (FAILED(hresult)) {
		return FALSE;
	}

	hresult = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

	if (FAILED(hresult)) {
		CoUninitialize();

		return FALSE;
	}

	hresult = CoCreateInstance(&CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
		&IID_IWbemLocator, (LPVOID *) & locator);

	if (FAILED(hresult)) {
		CoUninitialize();

		return FALSE;
	}

	namespace = SysAllocString(query_namespace);

	// Connect to the namespace with the current user and obtain pointer
	// services to make IWbemServices calls.
	hresult = locator->lpVtbl->ConnectServer(locator, namespace, NULL, NULL, NULL, 0,
		NULL, NULL, services);

	result = FAILED(hresult) ? FALSE : TRUE;

	SysFreeString(namespace);
	locator->lpVtbl->Release(locator);

	return result;
}

/**
 * Execute the suplied WMI query and call the row checking function for each row returned.
 */
int wmi_check_query(IWbemServices *services, const wchar_t *language, const wchar_t *query,
		wmi_check_row check_row) {
	int status = FALSE;
	IEnumWbemClassObject *queryrows = NULL;
	BSTR wmilang = SysAllocString(language);
	BSTR wmiquery = SysAllocString(query);

	// Execute the query.
	HRESULT result = services->lpVtbl->ExecQuery(
		services, wmilang, wmiquery, WBEM_FLAG_BIDIRECTIONAL, NULL, &queryrows);

	if (!FAILED(result) && (queryrows != NULL)) {
		IWbemClassObject * batchrows[10];
		ULONG index, count = 0;
		result = WBEM_S_NO_ERROR;

		while (WBEM_S_NO_ERROR == result && status == FALSE) {
			// Retrieve 10 rows (instances) each time.
			result = queryrows->lpVtbl->Next(queryrows, WBEM_INFINITE, 10,
				batchrows, &count);

			if (!SUCCEEDED(result)) {
				continue;
			}

			for (index = 0; index < count && status == FALSE; index++) {
				status = check_row(batchrows[index]);

				batchrows[index]->lpVtbl->Release(batchrows[index]);
			}
		}

		queryrows->lpVtbl->Release(queryrows);
	}

	SysFreeString(wmiquery);
	SysFreeString(wmilang);

	return status;
}

/**
 * Cleanup WMI.
 */
void wmi_cleanup(IWbemServices *services) {
	if (services != NULL) {
		services->lpVtbl->Release(services);
	}

	CoUninitialize();
}
