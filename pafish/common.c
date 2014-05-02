
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "common.h"

int analysis_result = 0;

void init_cmd_colors() {
     HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
     SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
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
     analysis_result = 2;
}

void print_not_traced() {
     HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
     SetConsoleTextAttribute(handler, 10);
     printf("OK\n");
     SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
}

void print_suspicious() {
     HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
     SetConsoleTextAttribute(handler, 207);
     printf("suspicious\n");
     SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
     if (analysis_result == 0) {
        analysis_result = 1;
     }
}

void write_log(char msg[]) {
    FILE *log;
    char logstr[1024];
    snprintf(logstr, sizeof(logstr), "\n[pafish] %s", msg);
    log = fopen("pafish.log", "a");
    fputs(logstr, log);
    fclose(log);
}

void write_trace(char product[]) {
    FILE *trace;
    trace = fopen(product, "a");
    fclose(trace);
}
