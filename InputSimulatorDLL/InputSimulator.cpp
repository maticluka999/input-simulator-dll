#include "pch.h"

#include "InputSimulator.h"

#include <string>

__declspec(dllexport) void __cdecl setCursorPos(int x, int y)
{
	SetCursorPos(x, y);
}

__declspec(dllexport) void __cdecl leftClick(int x, int y)
{
	SetCursorPos(x, y);
	Sleep(200);
	INPUT inputs[2];

	inputs[0] = {};
	inputs[0].type = INPUT_MOUSE;
	inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

	inputs[1] = {};
	inputs[1].type = INPUT_MOUSE;
	inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

	SendInput(2, inputs, sizeof(INPUT));
	Sleep(100);
}

__declspec(dllexport) void __cdecl typeTextFromKeyboard(const char* text, int length)
{
	HKL keyboardLayout = GetKeyboardLayout(0);
	INPUT inputs[3];

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_RSHIFT;
	inputs[0].ki.dwFlags = KEYEVENTF_EXTENDEDKEY;

	for (int i = 0; i < length; i++)
	{
		inputs[1].type = INPUT_KEYBOARD;
		short key = VkKeyScanExA(text[i], keyboardLayout);
		inputs[1].ki.wVk = key;
		inputs[1].ki.dwFlags = KEYEVENTF_UNICODE;

		inputs[2] = inputs[0];
		inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

		if (text[i] != '#' && text[i] != '?' && text[i] != '_')
			SendInput(1, &inputs[1], sizeof(INPUT));
		else
			SendInput(3, inputs, sizeof(INPUT));

		if (text[i + 1] == text[i])
			Sleep(150);
		else
			Sleep(50);
	}
}

__declspec(dllexport) void __cdecl rightClick(int x, int y)
{
	SetCursorPos(x, y);
	Sleep(100);
	INPUT inputs[2];

	inputs[0] = {};
	inputs[0].type = INPUT_MOUSE;
	inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

	inputs[1] = {};
	inputs[1].type = INPUT_MOUSE;
	inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

	SendInput(2, inputs, sizeof(INPUT));
	Sleep(100);
}

__declspec(dllexport) void __cdecl rollMouseWheel(int amount)
{
	INPUT input = {};
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_WHEEL;
	input.mi.mouseData = amount;
	SendInput(1, &input, sizeof(INPUT));
}

__declspec(dllexport) void __cdecl pressTab()
{
	INPUT input = {};
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = VK_TAB;
	input.ki.dwFlags = KEYEVENTF_UNICODE;
	SendInput(1, &input, sizeof(INPUT));
}

__declspec(dllexport) void __cdecl pressEnter()
{
	INPUT input = {};
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = VK_RETURN;
	input.ki.dwFlags = KEYEVENTF_UNICODE;
	SendInput(1, &input, sizeof(INPUT));
}

__declspec(dllexport) Color __cdecl getPixelColorUnderCursor()
{
	POINT p;
	COLORREF colorRef;
	HDC hDC;
	BOOL b;
	Color ret;

	// Get the device context for the screen
	hDC = GetDC(NULL);
	if (hDC == NULL)
	{
		ret.r = -1;
		ret.g = -1;
		ret.b = -1;

		return ret;
	}

	// Get the current cursor position
	b = GetCursorPos(&p);
	if (!b)
	{
		ret.r = -2;
		ret.g = -2;
		ret.b = -2;

		return ret;
	}

	// Retrieve the color at that position
	colorRef = GetPixel(hDC, p.x, p.y);
	if (colorRef == CLR_INVALID)
	{
		ret.r = -3;
		ret.g = -3;
		ret.b = -3;

		return ret;
	}

	// Release the device context again
	ReleaseDC(GetDesktopWindow(), hDC);

	ret.r = GetRValue(colorRef);
	ret.g = GetGValue(colorRef);
	ret.b = GetBValue(colorRef);

	return ret;
}

__declspec(dllexport) const char* __cdecl getClipboardText()
{
	// Try opening the clipboard
	if (!OpenClipboard(nullptr))
		return "error";

	// Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData(CF_TEXT);

	if (hData == nullptr)
		return "error";

	// Lock the handle to get the actual text pointer
	char* pszText = static_cast<char*>(GlobalLock(hData));

	if (pszText == nullptr)
		return "error";

	/*
	// Save text in a string class instance
	std::string text(pszText);
	*/

	// Release the lock
	GlobalUnlock(hData);

	// Release the clipboard
	CloseClipboard();
	
	return pszText;
}
