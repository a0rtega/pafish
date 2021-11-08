
#ifndef UTILS_H
#define UTILS_H

#include <wbemidl.h>
#include <windows.h>
#include <winternl.h>

#ifndef _WDMDDK_
typedef struct _RTL_BITMAP {
	ULONG SizeOfBitMap;
	PULONG Buffer;
} RTL_BITMAP, *PRTL_BITMAP;
#endif

/*
  PEB data structure
  https://github.com/wine-mirror/wine/blob/1aff1e6a370ee8c0213a0fd4b220d121da8527aa/include/winternl.h#L268
*/
typedef struct _PEB_wine
{                                                                 /* win32/win64 */
	BOOLEAN                      InheritedAddressSpace;             /* 000/000 */
	BOOLEAN                      ReadImageFileExecOptions;          /* 001/001 */
	BOOLEAN                      BeingDebugged;                     /* 002/002 */
	BOOLEAN                      SpareBool;                         /* 003/003 */
	HANDLE                       Mutant;                            /* 004/008 */
	HMODULE                      ImageBaseAddress;                  /* 008/010 */
	PPEB_LDR_DATA                LdrData;                           /* 00c/018 */
	RTL_USER_PROCESS_PARAMETERS *ProcessParameters;                 /* 010/020 */
	PVOID                        SubSystemData;                     /* 014/028 */
	HANDLE                       ProcessHeap;                       /* 018/030 */
	PRTL_CRITICAL_SECTION        FastPebLock;                       /* 01c/038 */
	PVOID /*PPEBLOCKROUTINE*/    FastPebLockRoutine;                /* 020/040 */
	PVOID /*PPEBLOCKROUTINE*/    FastPebUnlockRoutine;              /* 024/048 */
	ULONG                        EnvironmentUpdateCount;            /* 028/050 */
	PVOID                        KernelCallbackTable;               /* 02c/058 */
	ULONG                        Reserved[2];                       /* 030/060 */
	PVOID /*PPEB_FREE_BLOCK*/    FreeList;                          /* 038/068 */
	ULONG                        TlsExpansionCounter;               /* 03c/070 */
	PRTL_BITMAP                  TlsBitmap;                         /* 040/078 */
	ULONG                        TlsBitmapBits[2];                  /* 044/080 */
	PVOID                        ReadOnlySharedMemoryBase;          /* 04c/088 */
	PVOID                        ReadOnlySharedMemoryHeap;          /* 050/090 */
	PVOID                       *ReadOnlyStaticServerData;          /* 054/098 */
	PVOID                        AnsiCodePageData;                  /* 058/0a0 */
	PVOID                        OemCodePageData;                   /* 05c/0a8 */
	PVOID                        UnicodeCaseTableData;              /* 060/0b0 */
	ULONG                        NumberOfProcessors;                /* 064/0b8 */
	ULONG                        NtGlobalFlag;                      /* 068/0bc */
} PEB_wine, *PPEB_wine;

int pafish_disable_wow64_fs_redirection(void * old);

int pafish_revert_wow64_fs_redirection(void * old);

int pafish_iswow64();

struct _PEB_wine * pafish_get_PEB();

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
