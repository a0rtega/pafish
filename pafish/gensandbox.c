
#define _WIN32_WINNT 0x0501 /* _WIN32_WINNT_WINXP */

#include <windows.h>
#include <winioctl.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

#include "types.h"
#include "gensandbox.h"
#include "utils.h"

/**
 * Prototype for IsNativeVhdBoot, which is only available in >= Windows 8
 */
typedef BOOL (WINAPI * IsNativeVhdBoot) (BOOL *);

int gensandbox_mouse_act() {
	POINT position1, position2;
	GetCursorPos(&position1);
	Sleep(2000); /* Sleep time */
	GetCursorPos(&position2);
	if ((position1.x == position2.x) && (position1.y == position2.y)) {
		/* No mouse activity during the sleep */
		return TRUE;
	}
	else {
		/* Mouse activity during the sleep */
		return FALSE;
	}
}

int gensandbox_username() {
	char username[200];
	size_t i;
	DWORD usersize = sizeof(username);
	GetUserName(username, &usersize);
	for (i = 0; i < strlen(username); i++) { /* case-insensitive */
		username[i] = toupper(username[i]);
	}
	if (strstr(username, "SANDBOX") != NULL) {
		return TRUE;
	}
	if (strstr(username, "VIRUS") != NULL) {
		return TRUE;
	}
	if (strstr(username, "MALWARE") != NULL) {
		return TRUE;
	}
	return FALSE;
}

int gensandbox_path() {
	char path[500];
	size_t i;
	DWORD pathsize = sizeof(path);
	GetModuleFileName(NULL, path, pathsize);
	for (i = 0; i < strlen(path); i++) { /* case-insensitive */
		path[i] = toupper(path[i]);
	}
	if (strstr(path, "\\SAMPLE") != NULL) {
		return TRUE;
	}
	if (strstr(path, "\\VIRUS") != NULL) {
		return TRUE;
	}
	if (strstr(path, "SANDBOX") != NULL) {
		return TRUE;
	}
	return FALSE;
}

int gensandbox_common_names() {
	DWORD dwSize = MAX_PATH;
	char szLogicalDrives[MAX_PATH] = {0};
	DWORD dwResult = GetLogicalDriveStrings(dwSize,szLogicalDrives);
	BOOL exists;

	if (dwResult > 0 && dwResult <= MAX_PATH)
	{
		char* szSingleDrive = szLogicalDrives;
		char filename[MAX_PATH] = {0};
		while(*szSingleDrive)
		{
			if (GetDriveType(szSingleDrive) != DRIVE_REMOVABLE ) {
				snprintf(filename, MAX_PATH, "%ssample.exe",szSingleDrive);
				exists = pafish_exists_file(filename);
				if (exists) return TRUE;
				snprintf(filename, MAX_PATH, "%smalware.exe",szSingleDrive);
				exists = pafish_exists_file(filename);
				if (exists) return TRUE;
			}
			szSingleDrive += strlen(szSingleDrive) + 1;
		}
	}
	return FALSE;
}

int gensandbox_drive_size() {
	HANDLE drive;
	BOOL result;
	GET_LENGTH_INFORMATION size;
	DWORD lpBytesReturned;

	drive = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (drive == INVALID_HANDLE_VALUE) {
		// Someone is playing tricks. Or not enough privileges.
		CloseHandle(drive);
		return FALSE;
	}
	result = DeviceIoControl(drive, IOCTL_DISK_GET_LENGTH_INFO, NULL, 0, &size,
	sizeof(GET_LENGTH_INFORMATION), &lpBytesReturned, NULL);
	CloseHandle(drive);
	if (result != 0) {
		if (size.Length.QuadPart / 1073741824 <= 60) /* <= 60 GB */
		return TRUE;
	}
	return FALSE;
}

int gensandbox_drive_size2() {
	ULARGE_INTEGER total_bytes;

	if (GetDiskFreeSpaceExA("C:\\", NULL, &total_bytes, NULL))
	{
		if (total_bytes.QuadPart / 1073741824 <= 60) /* <= 60 GB */
		return TRUE;
	}
	return FALSE;
}

int gensandbox_sleep_patched() {
	DWORD time1;

	time1 = GetTickCount();
	Sleep(500);
	if ((GetTickCount() - time1) > 450 ) return FALSE;
	else return TRUE;
}

int gensandbox_one_cpu() {
	DWORD NumberOfProcessors = 0;
	__asm__ volatile (
			"mov %%fs:0x18, %%eax;"
			"mov %%ds:0x30(%%eax), %%eax;"
			"mov %%ds:0x64(%%eax), %%eax;"
			: "=a"(NumberOfProcessors));
	return NumberOfProcessors < 2 ? TRUE : FALSE;
}

int gensandbox_one_cpu_GetSystemInfo() {
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);
	return siSysInfo.dwNumberOfProcessors < 2 ? TRUE : FALSE;
}

int gensandbox_less_than_onegb() {
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);

	GlobalMemoryStatusEx(&statex);
	return (statex.ullTotalPhys/1024) < 1048576 ? TRUE : FALSE;
}

int gensandbox_uptime() {
	/* < ~12 minutes */
	return GetTickCount() < 0xAFE74 ? TRUE : FALSE;
}

int gensandbox_IsNativeVhdBoot() {
	BOOL isnative = FALSE;
	IsNativeVhdBoot fnnative = (IsNativeVhdBoot) GetProcAddress(
			GetModuleHandleA("kernel32"), "IsNativeVhdBoot");
	/* IsNativeVhdBoot always returns 1 on query success */
	if (fnnative)
		fnnative(&isnative);
	return (isnative) ? TRUE : FALSE;
}

