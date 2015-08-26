
#include <windows.h>

#include "bochs.h"
#include "types.h"
#include "utils.h"

int bochs_reg_key1() {
	return pafish_exists_regkey_value_str(HKEY_LOCAL_MACHINE, "HARDWARE\\Description\\System", "SystemBiosVersion", "BOCHS");
}

