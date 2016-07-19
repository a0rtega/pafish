
#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "cuckoo.h"

/**
 * Cuckoo Sandbox definitions.
 */
/**
 * Extra space allocated with the hooks information structure.
 */
#define TLS_HOOK_INFO_RETADDR_SPACE 0x100

/**
 * Hook information stored by Cuckoo at FS:[TLS_HOOK_INFO].
 */
struct hook_info {
	unsigned int depth_count;
	unsigned int hook_count;
	unsigned int retaddr_esp;
	unsigned int last_error;
	unsigned int ret_last_error;
	unsigned int eax;
	unsigned int ecx;
	unsigned int edx;
	unsigned int ebx;
	unsigned int esp;
	unsigned int ebp;
	unsigned int esi;
	unsigned int edi;
};

/**
 * Read the address of the hooks information in the TLS.
 */
struct hook_info *read_hook_info() {
	void *result = NULL;

	__asm__ volatile ("mov %%fs:0x44,%%eax" : "=a" (result));

	return result;
}

/**
 * Cuckoo stores the return addresses in a extra space allocated in conjunction
 * with the hook information function. The only way to check if the structure
 * is valid is to calculate what is the minimum and maximum value for the
 * return address value location.
 */
int cuckoo_check_tls() {
	struct hook_info *info = read_hook_info();

	if (info == NULL) {
		return FALSE;
	}

	unsigned int minimum = ((unsigned int) info + sizeof(struct hook_info));
	unsigned int maximum = minimum + TLS_HOOK_INFO_RETADDR_SPACE;

	return (info != NULL) && (info->retaddr_esp >= minimum && info->retaddr_esp <= maximum) ?
		TRUE : FALSE;
}
