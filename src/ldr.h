#pragma once

#include "pch.h"
#include "classes/list.h"
#include "classes/options.h"
#define BTN_CALLBACK(__fcnName) void __stdcall __fcnName(void* pSender)
#define COCOS_HOOK __fastcall

namespace ldr {
	namespace gates {
		inline void(__thiscall* loadingFinished)(cocos2d::CCScene*);
		inline void(__thiscall* dataLoaded)(cocos2d::CCObject*, void*);
		inline void(__thiscall* trySaveGame)(cocos2d::CCObject*);
		inline void(__thiscall* addSearchPath)(cocos2d::CCFileUtils*, const char*);
	}

	namespace hooks {
		void COCOS_HOOK loadingFinished(cocos2d::CCScene* LoadingLayer);
		void COCOS_HOOK dataLoaded(cocos2d::CCObject* GameManager, void*, void* DS_Dictionary);
		void COCOS_HOOK trySaveGame(cocos2d::CCObject* AppDelegate);
		void COCOS_HOOK addSearchPath(cocos2d::CCFileUtils* _this, void*, const char* path);
	}

	BTN_CALLBACK(enterScene);
	bool init();
}
