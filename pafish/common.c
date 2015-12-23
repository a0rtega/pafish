
#define _WIN32_WINNT 0x0501 /* _WIN32_WINNT_WINXP */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "config.h"
#include "common.h"
#include "types.h"

unsigned short init_cmd_colors() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
	// Get original console colors
	GetConsoleScreenBufferInfo(handler, &csbi);
	SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
	// Return original console colors
	return csbi.wAttributes;
}

void restore_cmd_colors(unsigned short original_colors) {
	HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
	// Restore original console colors
	SetConsoleTextAttribute(handler, original_colors);
}

void print_header() {
	HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("* Pafish (");
	SetConsoleTextAttribute(handler, 11);
	printf("Paranoid fish");
	SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
	printf(") *\n\n");
	printf("Some anti(debugger/VM/sandbox) tricks\n");
	printf("used by malware for the general public.\n\n");
}

void print_traced() {
	HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handler, 207);
	printf("traced!\n");
	SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
}

void print_not_traced() {
	HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handler, 10);
	printf("OK\n");
	SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
}

void print_warning(char * msg) {
	HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handler, 224);
	printf("%s\n", msg);
	SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
}

void write_log(char msg[]) {
	FILE *log;
	char logstr[1024];
	snprintf(logstr, sizeof(logstr)-sizeof(logstr[0]), "\n[pafish] %s", msg);
	log = fopen("pafish.log", "a");
	fputs(logstr, log);
	fclose(log);
}

void write_trace(char product[]) {
	FILE *trace;
	trace = fopen(product, "a");
	fclose(trace);
	#if ENABLE_DNS_TRACE
		write_trace_dns(product);
	#endif
}

void write_trace_dns(char product[]) {
	char * dns, tld[] = ".pafish";
	int i;
	struct addrinfo* result;
	int error;

	dns = calloc(strlen(product) + strlen(tld) + sizeof(char), sizeof(char));
	strncpy(dns, product, strlen(product));
	strncat(dns, tld, strlen(tld));
	for (i = 0; i < (int)strlen(dns); i++) {
		if (dns[i] == '_')
			dns[i] = '-';
	}

	error = getaddrinfo(dns, NULL, NULL, &result);
	if (!error)
		freeaddrinfo(result);
	free(dns);
}

void print_check_group(char * text) {
	printf("\n[-] %s\n", text);
}

void exec_check(char * text, int (*callback)(), char * text_log, char * text_trace) {
	int check_result;
	int (*callback_writeslog)(int) = callback;

	/* Handle functions that write logs */
	if (text_log)
		check_result = callback();
	else
		check_result = callback_writeslog(TRUE);

	printf("[*] %s ... ", text);
	if (check_result == TRUE) {
		/* Some checks write their own logs */
		if (text_log)
			write_log(text_log);
		print_traced();
		write_trace(text_trace);
	}
	else print_not_traced();
}

