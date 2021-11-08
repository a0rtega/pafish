
#define _WIN32_WINNT 0x0501 /* _WIN32_WINNT_WINXP */

#include <windows.h>

#include "debuggers.h"
#include "utils.h"
#include "types.h"

int debug_isdebuggerpresent() {
	if (IsDebuggerPresent())
	return TRUE;
	else
	return FALSE;
}

int debug_outputdebugstring() {
	DWORD err = 99; /* Random error */
	SetLastError(err);
	/* If we're been debugging, this shouldn't
	drop an error. */
	OutputDebugString("useless");
	if (GetLastError() == err){
		return TRUE;
	}
	else {
		return FALSE;
	}
}

int debug_beingdebugged_peb() {
	struct _PEB_wine * PEB;
	PEB = pafish_get_PEB();
	return PEB->BeingDebugged == 1 ? TRUE : FALSE;
}
