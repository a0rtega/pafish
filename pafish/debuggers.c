
#define _WIN32_WINNT 0x0501 /* _WIN32_WINNT_WINXP */

#include <windows.h>

#include "debuggers.h"

int debug_isdebuggerpresent() {
    if (IsDebuggerPresent()) {
        return 0;
    }
    else {
        return 1;
    }
}

/* This function is not used because it isn't reliable in
   some new environments */
int debug_checkremotedebuggerpresent() {
    BOOL isdebug = FALSE;
    CheckRemoteDebuggerPresent(GetCurrentProcess(), &isdebug);
    if (isdebug) {
        return 0;
    }
    else {
        return 1;
    }
}

int debug_outputdebugstring() {
    DWORD err = 99; /* Random error */
    SetLastError(err);
    /* If we're been debugging, this shouldn't
       drop an error. */
    OutputDebugString("useless");
    if (GetLastError() == err){
        return 0;
    }
    else {
        return 1;
    }
}
