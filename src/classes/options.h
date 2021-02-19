#pragma once

#include "pch.h"

static class options {
private:
	static inline gd::GJDropDownLayer* m_layer;

public:
	static void __stdcall enter(void* pSender);
};

