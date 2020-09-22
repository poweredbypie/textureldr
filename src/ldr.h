#pragma once
#include "gd.h"
#include "cocos.h"
#define BTN_CALLBACK(__fcnName) void __stdcall __fcnName(void* pSender)
#define COCOS_HOOK void __fastcall

namespace ldr {
	namespace vars {
		extern cocos::fileUtils::tAddPath gAddPath;
	}

	namespace hooks {
		COCOS_HOOK addPath(void* CCFileUtils, void* __EDX, const char* path);
		COCOS_HOOK loadingFinished(void* LoadingLayer, void* __EDX);
	}

	BTN_CALLBACK(enterScene);
	bool init();
}
