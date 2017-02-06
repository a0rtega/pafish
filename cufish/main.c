
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wbemidl.h>

#include "types.h"
#include "common.h"
#include "wmi.h"
#include "fingerprint.h"

/*
	Cufish (Curious fish)

	All code from this project, including
	functions, procedures and the main program
	is licensed under GNU/GPL version 3.

	So, if you are going to use functions or
	procedures from this project to develop
	your malware, you have to release the
	source code as well :)

	- Alberto Ortega
	- Duarte Silva

	Green fish icon thanks to http://www.fasticon.com/

*/

int main(void)
{
	unsigned short original_colors = 0;
	IWbemServices *services = NULL;
	int index = 0;

	write_log(L"Start");

	original_colors = init_cmd_colors();
	print_header();

	printf("[*] Initializing socket ... ");
	if (init_socket() != TRUE) {
		print_failed();
	}
	else {
		print_ok();

		printf("\n[-] Obtaining data using WMI\n");
		printf("[*] Starting WMI client ... ");
		if (wmi_initialize(&services) != TRUE) {
			print_failed();
		}
		else {
			print_ok();

			for (index = 0; wmitargets[index].caption != NULL; index++) {
				wprintf(L"[*] Obtaining %s data ... ", wmitargets[index].caption);
				if (wmi_execute_query(services, wmitargets[index].caption, wmitargets[index].classname,
						wmitargets[index].properties) != TRUE) {
					print_failed();
				}
				else print_ok();
			}

			printf("[*] Cleanup WMI client ... \n");
			wmi_cleanup(services);
		}

		printf("\n[*] Cleanup socket ...\n");
		clean_socket();
	}

	printf("\n\n");
	printf("[-] Feel free to RE me, check log file for more information.");

	write_log(L"End");

	getchar();

	/* Restore Original Console Colors */
	restore_cmd_colors(original_colors);

	return 0;
}
