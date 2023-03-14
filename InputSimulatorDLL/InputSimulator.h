#pragma once

#include <string>
#include <Windows.h>

#include "pch.h"

struct Color
{
	int r; // red 
	int g; // green
	int b; // blue
};

extern "C"
{
	__declspec(dllexport) void __cdecl setCursorPos(int x, int y);
	__declspec(dllexport) void __cdecl leftClick(int x, int y);
	__declspec(dllexport) void __cdecl typeTextFromKeyboard(const char* text, int length);
	__declspec(dllexport) void __cdecl rightClick(int x, int y);
	__declspec(dllexport) void __cdecl rollMouseWheel(int amount);
	__declspec(dllexport) void __cdecl pressTab();
	__declspec(dllexport) void __cdecl pressEnter();
	__declspec(dllexport) Color __cdecl getPixelColorUnderCursor();
	__declspec(dllexport) const char* __cdecl getClipboardText();
}
