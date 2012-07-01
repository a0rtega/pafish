
#include <windows.h>

int debug_isdebuggerpresent() {
    if (IsDebuggerPresent()) {
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
