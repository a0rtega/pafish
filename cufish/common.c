
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "common.h"

/**
 * The server address to send the logs to.
 */
static const char *server_addr = "192.168.1.1";
/**
 * The server port.
 */
static const int server_port = 9876;
/**
 * Socket to be used to send the logs from.
 */
SOCKET client_socket = INVALID_SOCKET;
/**
 * Socket address to send the logs to.
 */
struct sockaddr_in socket_addr;

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
	printf("* Cufish (");
	SetConsoleTextAttribute(handler, 10);
	printf("Curious fish");
	SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
	printf(") *\n\n");
	printf("Fingerprinting malware execution\n");
	printf("environments.\n\n");
}

void print_ok() {
	HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handler, 10);
	printf("OK\n");
	SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
}

void print_failed() {
	HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handler, 207);
	printf("failed!\n");
	SetConsoleTextAttribute(handler, FOREGROUND_INTENSITY);
}

/**
 * Write the log messane into a file.
 */
void write_log(wchar_t *message) {
	FILE *log = fopen("cufish.log", "a");
	fwprintf(log, L"\n[cufish] %s", message);
	fclose(log);
}

/**
 * Send the log message over the UDP socket. This function will convert the
 * message from a wide character string to a multiple byte string making it
 * easier to read when inspecting the network packet captures.
 */
void send_log(wchar_t *message) {
	size_t length = wcslen(message) + 2; 

	// Get a nice zero filled string.
	char *final = malloc(length * sizeof(char));
	memset(final, 0, length * sizeof(char));

	wcstombs(final, message, length);
	strcat(final, "\n");

	sendto(client_socket, final, strlen(final) * sizeof(char), 0,
		(SOCKADDR *) &socket_addr, sizeof(struct sockaddr_in));

	free(final);
}

/**
 * Initialize the UDP socket. Return TRUE if everything works out, FALSE
 * otherwise.
 */
int init_socket() {
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
		return FALSE;
	}

	if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
		WSACleanup();
		return FALSE;
	}

	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = htons(server_port);
	socket_addr.sin_addr.s_addr = inet_addr(server_addr);

	return TRUE;
}

/**
 * Cleanup the UDP socket.
 */
void clean_socket() {
	if (client_socket == INVALID_SOCKET) {
		closesocket(client_socket);
	}

	WSACleanup();
}
