#pragma once

#include "pch.h"
#include "classes/list.h"
#define BTN_CALLBACK(__fcnName) void __stdcall __fcnName(void* pSender)
#define COCOS_HOOK __fastcall

namespace ldr {
	namespace gates {
		inline void(__thiscall* addSearchPath)(cocos2d::CCFileUtils*, const char* path);
		inline void(__thiscall* trySaveGame)(cocos2d::CCNode* AppDelegate);
	}

	namespace hooks {
		void COCOS_HOOK addSearchPath(cocos2d::CCFileUtils* _this, void* __EDX, const char* path);
		void COCOS_HOOK loadingFinished(cocos2d::CCScene* LoadingLayer);
		void COCOS_HOOK trySaveGame(cocos2d::CCNode* AppDelegate);
	}

	BTN_CALLBACK(enterScene);
	bool init();
}
