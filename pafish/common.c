
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "common.h"

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
}
