#pragma once

#include "pch.h"
#include "extern/gd.h"
#include "extern/cocos.h"
#include "classes/list.h"
#define BTN_CALLBACK(__fcnName) void __stdcall __fcnName(void* pSender)
#define COCOS_HOOK __fastcall

namespace ldr {
	namespace gates {
		extern void(__thiscall* addPath)(void* CCFileUtils, const char* path);
		extern void(__thiscall* trySaveGame)(void* AppDelegate);
	}

	namespace hooks {
		void COCOS_HOOK addPath(void* CCFileUtils, void* __EDX, const char* path);
		void COCOS_HOOK loadingFinished(void* LoadingLayer);
		void COCOS_HOOK trySaveGame(void* AppDelegate);
	}

	BTN_CALLBACK(enterScene);
	bool init();
}
