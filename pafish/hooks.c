
#include <windows.h>

#include "hooks.h"
#include "types.h"

/* Thx Inaki for this! (@virtualminds_es) */
static int check_hook_m1(DWORD * dwAddress) {
	BYTE *b = (BYTE *)dwAddress;
	return (*b == 0x8b) && (*(b+1) == 0xff) ? FALSE : TRUE;
}

/* Causes FP in Win 8 */
int check_hook_DeleteFileW_m1() {
	return check_hook_m1((DWORD *)DeleteFileW);
}

int check_hook_ShellExecuteExW_m1() {
	return check_hook_m1((DWORD *)ShellExecuteExW);
}

int check_hook_CreateProcessA_m1() {
	return check_hook_m1((DWORD *)CreateProcessA);
}

