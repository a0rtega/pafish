
#include <windows.h>
#include <string.h>

#include "qemu.h"
#include "types.h"
#include "utils.h"
#include "cpu.h"

int qemu_reg_key1() {
	return pafish_exists_regkey_value_str(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", "Identifier", "QEMU");
}

int qemu_reg_key2() {
	return pafish_exists_regkey_value_str(HKEY_LOCAL_MACHINE, "HARDWARE\\Description\\System", "SystemBiosVersion", "QEMU");
}

int qemu_cpu_name() {
	char cpu_brand[49];
	cpu_write_brand(cpu_brand);
	return !memcmp(cpu_brand, "QEMU Virtual CPU", 16) ? TRUE : FALSE;
}

