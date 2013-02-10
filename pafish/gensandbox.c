
#include <windows.h>
#include <string.h>

#include "gensandbox.h"

int gensandbox_mouse_act() {
    POINT position1, position2;
    GetCursorPos(&position1);
    Sleep(1000); /* Sleep time */
    GetCursorPos(&position2);
    if ((position1.x == position2.x) && (position1.y == position2.y)) {
        /* No mouse activity during the sleep */
        return 0;
    }
    else {
        /* Mouse activity during the sleep */
        return 1;
    }
}

int gensandbox_username() {
    char username[200];
    int i;
    DWORD usersize = sizeof(username);
    GetUserName(username, &usersize);
    for (i = 0; i < strlen(username); i++) { /* Uppercase to case-insensitive */
        username[i] = toupper(username[i]);
    }
    if (strstr(username, "SANDBOX") != NULL) {
        return 0;
    }
    if (strstr(username, "VIRUS") != NULL) {
        return 0;
    }
    if (strstr(username, "MALWARE") != NULL) {
        return 0;
    }
    return 1;
}

int gensandbox_path() {
    char path[500];
    int i;
    DWORD pathsize = sizeof(path);
    GetModuleFileName(NULL, path, pathsize);
    for (i = 0; i < strlen(path); i++) { /* Uppercase to case-insensitive */
        path[i] = toupper(path[i]);
    }
    if (strstr(path, "\\SAMPLE") != NULL) {
        return 0;
    }
    if (strstr(path, "\\VIRUS") != NULL) {
        return 0;
    }
    if (strstr(path, "SANDBOX") != NULL) {
        return 0;
    }
    return 1;
}
