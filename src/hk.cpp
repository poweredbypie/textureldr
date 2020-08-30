#include "pch.h"
#include "hk.h"

void patch(char* dst, char* src, char* buff, const int len) {
	if (buff) {
		memcpy_s(buff, len, dst, len);
	}
	unsigned long old{};
	VirtualProtect(dst, len, PAGE_EXECUTE_READWRITE, &old);
	memcpy_s(dst, len, src, len);
	VirtualProtect(dst, len, old, &old);
}

bool detour(char* src, char* dst, const int len) {
	unsigned long old{};
	if (len < 5)
		return false;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &old);
	int jmp{ (int)(dst - src - 5) };
	*src = 0xE9;
	*(int*)(src + 1) = jmp;
	VirtualProtect(src, len, old, &old);
	return true;
}
char* trampoline(char* src, char* dst, const int len) {
	if (len < 5)
		return 0;
	char* gateway{ (char*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE) };
	memcpy_s(gateway, len, src, len);
	int jmp{ (int)(src - gateway - 5) };
	*(gateway + len) = 0xE9;
	*(int*)(gateway + len + 1) = jmp;
	detour(src, dst, len);
	return gateway;
}

void hk::hook() {
	memcpy_s(oldInstruct, len, src, len);
	if (pGate)
		*pGate = trampoline(src, dst, len);
	else
		trampoline(src, dst, len);
	on = true;
}
void hk::unhook() {
	unsigned long old{};
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &old);
	memcpy_s(src, len, oldInstruct, len);
	VirtualProtect(src, len, old, &old);
	if (pGate)
		VirtualFree(*pGate, 0, MEM_RELEASE);
	on = false;
}