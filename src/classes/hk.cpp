#include "pch.h"
#include "hk.h"

//some stupid wrappers to log
inline void memcpyLog(void* dst, const int dstLen, void const* src, const int srcLen) {
	errno_t err = memcpy_s(dst, dstLen, src, srcLen);
	if (err) {
		log::error("memcpy failed with error ", err);
	}
}

inline void virtualProtectLog(LPVOID dst, SIZE_T len, DWORD newProtect, PDWORD oldProtect) {
	if (!VirtualProtect(dst, len, newProtect, oldProtect)) {
		log::error("virtualprotect failed with error ", GetLastError());
	}
}

void patch(void* dst, void const* src, void* buff, const int len) {
	log::info("Attempting to patch bytes...");
	if (buff) {
		memcpyLog(buff, len, dst, len);
	}
	unsigned long old = 0;
	virtualProtectLog(dst, len, PAGE_EXECUTE_READWRITE, &old);
	memcpyLog(dst, len, src, len);
	virtualProtectLog(dst, len, old, &old);
}

bool detour(unsigned char* src, const unsigned char* dst, const int len) {
	unsigned long old = 0;
	if (len < 5)
		return false;
	virtualProtectLog(src, len, PAGE_EXECUTE_READWRITE, &old);
	int jmp = static_cast<int>(dst - src - 5);
	*src = 0xE9;
	*reinterpret_cast<int*>(src + 1) = jmp;
	virtualProtectLog(src, len, old, &old);
	return true;
}

unsigned char* trampoline(unsigned char* src, const unsigned char* dst, const int len) {
	if (len < 5)
		return 0;
	unsigned char* gateway = static_cast<unsigned char*>(VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
	memcpyLog(gateway, len, src, len);
	int jmp = static_cast<int>(src - gateway - 5);
	*(gateway + len) = 0xE9;
	*reinterpret_cast<int*>(gateway + len + 1) = jmp;
	detour(src, dst, len);
	return gateway;
}

void hk::hook() {
	log::info("Attempting to enable hook...");
	memcpyLog(oldInstruct, len, src, len);
	if (pGate)
		*pGate = trampoline(src, dst, len);
	else
		trampoline(src, dst, len);
	on = true;
}

void hk::unhook() {
	unsigned long old{};
	virtualProtectLog(src, len, PAGE_EXECUTE_READWRITE, &old);
	memcpyLog(src, len, oldInstruct, len);
	virtualProtectLog(src, len, old, &old);
	if (pGate)
		VirtualFree(*pGate, 0, MEM_RELEASE);
	on = false;
}