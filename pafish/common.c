
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void init_cmd_colors() {
     HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
     SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
}

void print_header() {
    HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("* Pafish (");
    SetConsoleTextAttribute(handler, FOREGROUND_GREEN);
    printf("Paranoid fish");
    SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
    printf(") *\n\n");
    printf("Some anti(debugger/VM/sandbox) tricks\n");
    printf("used by malware for the general public.\n\n");
    printf("- Author: Alberto Ortega (alberto[at]pentbox.net)\n\n");
}

void print_traced() {
     HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
     SetConsoleTextAttribute(handler, FOREGROUND_RED);
     printf("traced!\n");
     SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
}

void print_not_traced() {
     HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
     SetConsoleTextAttribute(handler, FOREGROUND_GREEN);
     printf("OK\n");
     SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
}

void print_suspicious() {
     HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
     SetConsoleTextAttribute(handler, FOREGROUND_RED);
     printf("suspicious\n");
     SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
}
