#pragma once

#include "pch.h"
#include "extern/gd.h"
#include "extern/cocos.h"
#include "classes/list.h"
#define BTN_CALLBACK(__fcnName) void __stdcall __fcnName(void* pSender)
#define COCOS_HOOK __fastcall
#define LIST_LENGTH 10

namespace ldr {
	namespace vars {
		extern cocos::fileUtils::tAddPath gAddPath;
	}

	namespace hooks {
		void COCOS_HOOK addPath(void* CCFileUtils, void* __EDX, const char* path);
		void COCOS_HOOK loadingFinished(void* LoadingLayer);
	}

	BTN_CALLBACK(enterScene);
	bool init();
}
