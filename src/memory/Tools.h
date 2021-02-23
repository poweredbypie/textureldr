#ifndef __HOOK_H__
#define __HOOK_H__

#include "pch.h"

inline void patch(void* dst, const void* src, void* buff, const int len) {
	Log::info("Attempting to patch bytes at ", std::hex, dst, '.');
	if (buff) {
		patch(buff, dst, nullptr, len);
	}
	if (!WriteProcessMemory(GetCurrentProcess(), dst, src, len, nullptr)) {
		Log::error("WriteProcessMemory failed with error ", std::hex, GetLastError(), '.');
	}
}

inline bool detour(void* src, void* dst, const int len) {
	if (len < 5) {
		Log::error("Length of buffer passed to detour is too small.");
		return false;
	}
	patch(src, "\xE9", nullptr, 1);
	int jmp = static_cast<char*>(dst) - static_cast<char*>(src) - 5;
	patch(static_cast<char*>(src) + 1, &jmp, nullptr, 4);
	return true;
}

void* trampoline(void* src, void* dst, const int len) {
	if (char* gate = static_cast<char*>(VirtualAlloc(
		nullptr, len,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE
	))) {
		patch(gate, src, nullptr, len);
		patch(gate + len, "\xE9", nullptr, 1);
		int jmp = static_cast<char*>(src) - gate - 5;
		patch(gate + len + 1, &jmp, nullptr, 4);
		detour(src, dst, len);
		return gate;
	}
	else {
		Log::error("VirtualAlloc failed with error ", GetLastError(), '.');
		return nullptr;
	}
}

class Hook {
private:
	bool on = false;
	void* src = nullptr;
	void* dst = nullptr;
	void** pGate = nullptr;
	const int len = 0;
	void** oldBytes = nullptr;

public:
	inline void enable() {
		Log::info("Attempting to enable hook.");
		patch(oldBytes, src, nullptr, len);
		*pGate = trampoline(src, dst, len);
		on = true;
	}

	inline void disable() {
		Log::info("Attempting to disable hook.");
		patch(src, oldBytes, nullptr, len);
		VirtualFree(*pGate, 0, MEM_RELEASE);
	}

	template<typename T, typename U, typename V>
	Hook(T src, U dst, V gate, const int len) :
		src{ src },
		dst{ dst },
		pGate{ reinterpret_cast<void**>(gate) },
		len{ len } {
		Log::info("New hook created.");
		oldBytes = new void* [len];
	}

	~Hook() {
		delete[] oldBytes;
	}
};

#endif