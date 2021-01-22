#pragma once

#include "pch.h"

void patch(void* dst, void const* src, void* buff, const int len);

bool detour(unsigned char* src, const unsigned char* dst, const int len);
unsigned char* trampoline(unsigned char* src, const unsigned char* dst, const int len);

class hk
{
	bool on = false;
	unsigned char* src = nullptr;
	const unsigned char* dst = nullptr;
	unsigned char** pGate = nullptr;
	const int len = 0;
	unsigned char** oldInstruct = nullptr;

public:
	void hook();
	void unhook();

	template<typename T, typename U, typename V>
	hk(T src, U dst, V gate, const int len) :
		src{ reinterpret_cast<unsigned char*>(src) },
		dst{ reinterpret_cast<unsigned char*>(dst) },
		pGate{ reinterpret_cast<unsigned char**>(gate) }, len{ len } {
		oldInstruct = new unsigned char* [len];
	};

	~hk() {
		delete[] oldInstruct;
	}
};

