
#include <windows.h>

#include "sandboxie.h"
#include "types.h"

int sboxie_detect_sbiedll() {
	if (GetModuleHandle("sbiedll.dll") != NULL) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}
