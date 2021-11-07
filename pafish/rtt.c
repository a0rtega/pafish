#include <windows.h>
#include <stdio.h>

#include "types.h"
#include "rtt.h"

/* Dialog's constants */
#define ID_OK 1
#define ID_QUIT 2
#define TEXT_OFF 20

/* Custom message indicating timer's end */
#define WM_CUSTOM (WM_USER + 0x0001)

/* Duration of each check */
#define MAX_DURATION 3000

/* Some RTT global variables */
HHOOK rtt_hook;
BOOL rtt_is_success = FALSE;
BOOL rtt_is_timeout = FALSE;
BOOL rtt_is_within_rect = FALSE;
/* Tracks the point in time of last click */
u_int64 rtt_last = 0;
/* Default double click time in milliseconds */
u_int rtt_double_click_time = 500;

/*
 * Checks, for the presence of a mouse device.
 *
 * Found in similar form in maldoc used by ColdRiver
 * MD5: 48320f502811645fa1f2f614bd8a385a
 */
int rtt_mouse_presence() {
	int res;
	res = GetSystemMetrics(SM_MOUSEPRESENT);
	return res > 0 ? FALSE : TRUE;
}

/*
 * Checks, if the mouse cursor moves.
 *
 * Found in similar form in IFSB/Gozi in version 2.13.24.1 leaked by vx-undeground.org
 * See: https://github.com/vxunderground/MalwareSourceCode/blob/main/Leaks/Win32/Win32.Gozi.rar
 */
int rtt_mouse_move() {
	POINT cur = { 0 }, prev = { 0 };
	u_int movement = 0;
	int interval = 100;
	int period = MAX_DURATION;

	do {
		GetCursorPos(&cur);

		if (prev.x && prev.y)
			movement = abs(cur.x - prev.x) + (abs(cur.y - prev.y) << 16);

		/* Cursor moved */
		if (movement) {
			return FALSE;
		}

		prev = cur;
		Sleep(interval);
		period -= interval;
	} while (period);

	return TRUE;
}

/*
 * Checks, for supernatural cursor speeds.
 *
 * Referenced in Fireeye Blogpost from 2014
 * https://www.fireeye.com/blog/threat-research/2014/06/turing-test-in-reverse-new-sandbox-evasion-techniques-seek-human-interaction.html
 */
int rtt_mouse_speed_limit() {
	POINT prev, cur;
	int mx, my, dx, dy;

	int period = MAX_DURATION;
	int interval = 10;
	int cursor_updates = 0;

	mx = GetSystemMetrics(SM_CXFULLSCREEN) / 5;
	my = GetSystemMetrics(SM_CYFULLSCREEN) / 5;

	GetCursorPos(&prev);

	while (period) {
		Sleep(interval); /* Sleep time */

		GetCursorPos(&cur);
		dx = prev.x - cur.x;
		dy = prev.y - cur.y;

		if ((dx != 0) || (dy != 0)) {
			/* Cursor moved */
			cursor_updates++;

			if (abs(dx) > mx && abs(dy) > my) {
				/* Cursor moved too fast */
				return TRUE;
			}
		}
		prev = cur;
		period -= interval;
	}

	/* Mouse actually moved, but never moved too fast, therefore pass check */
	if (cursor_updates) {
		return FALSE;
	}

	/* No mouse activity, fail check as well */
	return TRUE;
}

/*
 * Timer callback, which sends a custom message to the message loop, in order
 * to signal that the time is up.
 */
VOID CALLBACK timer_proc() {
	PostMessageA(NULL, WM_CUSTOM, 0, 0);
}

/*
 * Callback for a low-level mouse hook, which checks, if a single click occurs.
 */
LRESULT CALLBACK single_click_proc(int nCode, WPARAM wParam, LPARAM lp) {
	if (nCode >= 0) {
		if (wParam == WM_LBUTTONUP) {
			rtt_is_success = TRUE;
			PostMessageA(NULL, WM_CUSTOM, 0 , 0);
		}
	}
	return CallNextHookEx(rtt_hook, nCode, wParam, lp);
}

/*
 * Calculates the current time in milliseconds by querying system time as
 * FILETIME and converting it to Unix epoch format.
 *
 * Taken from https://gist.github.com/e-yes/278302
 */
u_int64 get_current_time_in_millis(){
	FILETIME ft;
	LARGE_INTEGER li;

	/*
	 * Gets the amount of 100 nano seconds intervals elapsed since January 1,
	 * 1601 (UTC) and copy it * to a LARGE_INTEGER structure.
	 */
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;

	u_int64 ms = li.QuadPart;
	ms -= 116444736000000000LL; /* Converts from file time to UNIX epoch time. */
	ms /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

	return ms;
}

/*
 * Callback for a low-level mouse hook, which checks, if a double click occurs.
 * The presence of a double click is assumed if two clicks are observed within
 * the time frame rtt_double_click_time.
 */
LRESULT CALLBACK double_click_proc(int code, WPARAM wp, LPARAM lp) {
	if (code >= 0) {
		if (wp == WM_LBUTTONDOWN) {
			u_int64 now = get_current_time_in_millis();
			if((now - rtt_last) < rtt_double_click_time){
				rtt_is_success = TRUE;
				PostMessageA(NULL, WM_CUSTOM, 0 , 0);
			}
			rtt_last = now;
		}
	}
	return CallNextHookEx(rtt_hook, code, wp, lp);
}

/*
 * Installs a low-level mouse hook with the specified callback.
 */
int install_hook(LRESULT CALLBACK (*callback)(int code, WPARAM wp, LPARAM lp)){
	SetTimer(NULL, 0, MAX_DURATION, (TIMERPROC) &timer_proc);
	rtt_hook = SetWindowsHookEx(WH_MOUSE_LL, callback, NULL, 0);
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		if (msg.message == WM_CUSTOM) {
			/* Timer ended, exit message loop*/
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	/* Clean up */
	KillTimer(NULL, 0);
	UnhookWindowsHookEx(rtt_hook);

	if (rtt_is_success)
		return FALSE;

	return TRUE;
}

/*
 * Checks for a single click with a technique used in the UpClicker trojan.
 * See https://webcache.googleusercontent.com/search?q=cache:NeVZ4J1Y-cQJ:https://www.fireeye.com/blog/threat-research/2012/12/dont-click-the-left-mouse-button-trojan-upclicker.html+&cd=1&hl=en&ct=clnk&gl=de
 */
int rtt_mouse_click() {
	return install_hook(&single_click_proc);
}

/*
 * Checks for dobule clicks.
 *
 * In the past several malware samples waited for a certain amount of clicks
 * (not necessarily double clicks) with different techniques, see for example:
 *
 * https://www.ptsecurity.com/ww-en/analytics/antisandbox-techniques/ mentioning
 * MyWeb backdoor used by APT15 in 2010, where three left clicks were required
 *
 * https://www.fireeye.com/blog/threat-research/2014/06/turing-test-in-reverse-new-sandbox-evasion-techniques-seek-human-interaction.html
 * referencing the use of GetAsyncKeyState()
 */
int rtt_mouse_double_click() {
	/* Determines double click time set on system */
	rtt_double_click_time = GetDoubleClickTime();
	/* Checks, if a double click occurs */
	return install_hook(&double_click_proc);
}

LRESULT CALLBACK timed_dialog_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

	RECT rect;
	int btn_w, btn_h;

	switch (msg) {
		case WM_CREATE:
			SetTimer(hwnd, 1, MAX_DURATION, NULL);

			/* Determines "usable" size inside of the current window */
			GetClientRect(hwnd, &rect);

			/* Calculates button size */
			btn_w = (rect.right - rect.left) / 2;
			btn_h = (rect.bottom - rect.top) / 2;

			/* Create two buttons - randomly chosen and "Quit" */
			CreateWindowW(L"Button", L"OK", WS_VISIBLE | WS_CHILD, 0, TEXT_OFF,
					btn_w, btn_h, hwnd, (HMENU) ID_OK, NULL, NULL);

			CreateWindowW(L"Button", L"Quit", WS_VISIBLE | WS_CHILD, 0 + btn_w,
					TEXT_OFF, btn_w, btn_h, hwnd, (HMENU) ID_QUIT, NULL, NULL);
			/* Ensure dialog is displayed at the very top */
			//SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			break;

		case WM_TIMER:
			rtt_is_timeout = TRUE;
			DestroyWindow(hwnd);
			break;

		case WM_COMMAND:

			{
				/*
				 * Plausibility check, whether cursor is still within rect.
				 * Buttons are not focusable, therefore pressing return is not in this case possible
				 */
				POINT p;
				GetCursorPos(&p);
				GetWindowRect(hwnd, &rect);

				if (p.x >= rect.left && p.x <= rect.right && p.y >= rect.top
						&& p.y <= rect.bottom)
					rtt_is_within_rect = TRUE;
			}

			/* Destroys and recreates a new window on "Ok" */
			if (LOWORD(wp) == ID_OK) {
				MessageBeep(MB_OK);
				DestroyWindow(hwnd);
			}

			/* Sets stop condition, if "Quit" was clicked */
			if (LOWORD(wp) == ID_QUIT) {
				DestroyWindow(hwnd);
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}

	return DefWindowProcW(hwnd, msg, wp, lp);
}

/*
 * Displays a dialog and waits for interaction
 */
int confirm_dialog(BOOL is_plausibility_check) {

	MSG msg;
	int mx, my, rx, ry, mw, mh;

	/* Defines class and window's attributes*/
	WNDCLASSW wc = { 0 };
	wc.lpszClassName = L"RTT window";
	wc.hInstance = NULL;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpfnWndProc = timed_dialog_proc;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	srand(GetTickCount());

	/* Size of screen without taskbar */
	mx = GetSystemMetrics(SM_CXFULLSCREEN);
	my = GetSystemMetrics(SM_CYFULLSCREEN);

	/* Limits for window size */
	mw = GetSystemMetrics(SM_CXMIN) * 3;
	mh = GetSystemMetrics(SM_CYMIN) * 3;

	/* Randomize window position */
	rx = ((double) rand() / RAND_MAX) * (mx - (4 * mw)) + mw;
	ry = ((double) rand() / RAND_MAX) * (my - (4 * mh)) + mh;

	RegisterClassW(&wc);
	CreateWindowW(wc.lpszClassName, L"Pafish RTT window",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CAPTION, rx, ry, mw,
			mh, 0, 0, NULL, 0);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (!rtt_is_timeout) {
		if (is_plausibility_check)
			return !rtt_is_within_rect;
		else
			return FALSE;
	}

	return TRUE;
}

/*
 * Wrapper function for displaying a dialog and checking if interaction occurs.
 *
 * Found in similar form in XLM-macro with MD5: 50d518246c2b61f5b427948f87a0aa24
 * See also: https://www.lastline.com/labsblog/evolution-of-excel-4-0-macro-weaponization/
 */
int rtt_confirm_dialog() {
	return confirm_dialog(FALSE);
}

/*
 * Wrapper function like rtt_confirm_dialog(), but performs an additional check
 * for plausibility by determining, whehter the cursor resides inside the
 * bounding box of the displayed dialog add the time of confirmation.
 * Thereby, machine-triggered SendMessageW-interactions can be identified.
 */
int rtt_plausible_confirm_dialog() {
	return confirm_dialog(TRUE);
}
