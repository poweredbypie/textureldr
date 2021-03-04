#ifndef __HOOKS_H__
#define __HOOKS_H__

#include "pch.h"

#include "../logic/nodes/LoaderManager.h"
#include "../logic/layers/LoaderLayer.h"

namespace addrs {
	inline auto ppMoreGamesBtn = reinterpret_cast<const char**>(gd::base + 0x190EF2);
	inline auto pSpriteSize = reinterpret_cast<float*>(gd::base + 0x190F01);
	inline auto ppCallback = reinterpret_cast<void**>(gd::base + 0x190F13);
	inline auto pFolderBtn = reinterpret_cast<const char*>(gd::base + 0x297B34);
	inline auto pNewCallback = LoaderLayer::scene;
}

namespace gates {
	inline void(__thiscall* LoadingLayer_loadingFinished)(gd::LoadingLayer*);
	inline void(__thiscall* GameManager_dataLoaded)(gd::GameManager*, void*);
	inline void(__thiscall* AppDelegate_trySaveGame)(cocos2d::CCObject*);
	inline void(__thiscall* CCFileUtils_addSearchPath)(cocos2d::CCFileUtils*, const char*);
}

namespace hooks {
	void __fastcall LoadingLayer_loadingFinished(gd::LoadingLayer* This) {
		This->setWillFadeIn(false);
		if (LoaderManager::sharedState()->m_bReloadSFX) {
			gd::FMODAudioEngine::sharedEngine()->reloadEffects();
		}
		return gates::LoadingLayer_loadingFinished(This);
	}

	void __fastcall GameManager_dataLoaded(gd::GameManager* This, void*, void* DS_Dictionary) {
		using namespace cocos2d;

		auto fileUtils = CCFileUtils::sharedFileUtils();
		//remove all added search paths.
		fileUtils->removeAllPaths();
		gates::CCFileUtils_addSearchPath(fileUtils, "Resources");
		//call dataLoaded.
		gates::GameManager_dataLoaded(This, DS_Dictionary);
		//add all of 'em back lol
		fileUtils->removeAllPaths();
		for (auto i : LoaderManager::sharedState()->m_dApplied.m_vEntries) {
			gates::CCFileUtils_addSearchPath(fileUtils, ("packs/" + i).c_str());
		}
		gates::CCFileUtils_addSearchPath(fileUtils, "Resources");
	}

	void __fastcall AppDelegate_trySaveGame(cocos2d::CCObject* This) {
		LoaderManager::sharedState()->save();
		return gates::AppDelegate_trySaveGame(This);
	}

	void __fastcall CCFileUtils_addSearchPath(cocos2d::CCFileUtils* This, void*, const char* path) {
		if (LoaderManager::sharedState()->m_bReloadMusic) {
			gd::MenuLayer::fadeInMusic(" ");
		}
		for (auto i : LoaderManager::sharedState()->m_dApplied.m_vEntries) {
			gates::CCFileUtils_addSearchPath(This, ("packs/" + i).c_str());
		}
		return gates::CCFileUtils_addSearchPath(This, path);
	}
}

#endif