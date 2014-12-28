
#include <windows.h>
#include <winioctl.h>
#include <string.h>

#include "gensandbox.h"

int gensandbox_mouse_act() {
    POINT position1, position2;
    GetCursorPos(&position1);
    Sleep(1750); /* Sleep time */
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
    for (i = 0; i < strlen(username); i++) { /* case-insensitive */
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
    for (i = 0; i < strlen(path); i++) { /* case-insensitive */
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

int gensandbox_drive_size() {
    HANDLE drive;
    BOOL result;
    GET_LENGTH_INFORMATION size;
    DWORD lpBytesReturned;
    
    drive = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (drive == INVALID_HANDLE_VALUE) {
        // Someone is playing tricks. Or not enough privileges.
        CloseHandle(drive);
        return 1;
    }
    result = DeviceIoControl(drive, IOCTL_DISK_GET_LENGTH_INFO, NULL, 0, &size,
                sizeof(GET_LENGTH_INFORMATION), &lpBytesReturned, NULL);
    CloseHandle(drive);
    if (result != 0) {
        if (size.Length.QuadPart / 1073741824 <= 50) { /* <= 50 GB */
            return 0;
        }
    }
    return 1;
}


int gensandbox_drive_size2() {
    ULARGE_INTEGER bytes_available;
    ULARGE_INTEGER total_bytes;
    ULARGE_INTEGER total_number_free_bytes;

    if (GetDiskFreeSpaceExA("C:\\", &bytes_available, &total_bytes, &total_number_free_bytes))
    {
        if (bytes_available.QuadPart / 1073741824 <= 60) { /* <= 60 GB */
            return 0;
        }
        if (total_bytes.QuadPart / 1073741824 <= 60) { /* <= 60 GB */
            return 0;
        }
        if (total_number_free_bytes.QuadPart / 1073741824 <= 60) { /* <= 60 GB */
            return 0;
        }
    }
    return 1;
}
