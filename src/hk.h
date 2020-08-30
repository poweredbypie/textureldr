#pragma once

void patch(char* dst, char* src, char* buff, const int len);

bool detour(const char* src, const char* dst, const int len);
char* trampoline(const char* src, const char* dst, const int len);

class hk
{
	bool on{};
	char* src{};
	char* dst{};
	char** pGate{};
	const int len{};
	char* oldInstruct[10]{};

public:
	void hook();
	void unhook();

	hk(char* src, char* dst, char** gate, const int len) : src{ src }, dst{ dst }, pGate{ gate }, len{ len } {};
};

