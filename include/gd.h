#pragma once

#include "pch.h"

#define as reinterpret_cast

#pragma runtime_checks("s", off)

namespace gd {
	inline auto base = as<char*>(GetModuleHandle(0));

	namespace AppDelegate {
		inline auto trySaveGame = as<void(__thiscall* const)(cocos2d::CCNode*)>(base + 0x3D5E0);
	}

	class GameManager : public cocos2d::CCNode {
	public:
		static GameManager* sharedState() {
			return as<GameManager* (__stdcall*)()>(
				base + 0xC4A50
				)();
		}

		void reloadAll(bool bSwitch, bool bFullscreen, bool bReloadedInSession) {
			return as<void(__thiscall*)(GameManager*, bool, bool, bool)>(
				base + 0xCE950
				)(this, bSwitch, bFullscreen, bReloadedInSession);
		}
	};

	class MenuLayer : public cocos2d::CCScene {
	public:
		static inline auto pMoreGamesStr = as<void const**>(base + 0x190EF2);
		static inline auto szMoreGamesBtn = as<float const**>(base + 0x190F01);
		static inline auto pcbMoreGames = as<void(__stdcall* const)(void*)>(base + 0x190F13);
		static inline auto oMoreGamesStr = as<char const*>(base + 0x2CD790);
		static inline auto oFolderBtnStr = as<char const*>(base + 0x297B34);
		static inline char* cbMoreGames = nullptr;

	public:
		static MenuLayer* create() {
			return as<MenuLayer* (__stdcall*)()>(
				base + 0x190550
				)();
		}

		static void fadeInMusic(const char* filename) {
			return as<void(__stdcall*)(const char*)>(
				base + 0xC4BD0
				)(filename);
		}
	};

	namespace LoadingLayer {
		inline auto loadingFinished = as<void(__thiscall* const)(cocos2d::CCScene*)>(base + 0x18C790);
	}

	#pragma runtime_checks("s", off)
	class ButtonSprite : public cocos2d::CCMenuItem {
	public:
		static ButtonSprite* create(cocos2d::CCSprite* sprite, cocos2d::CCMenu* target, void(__stdcall* const callback)(void*)) {
			auto pRet = as<ButtonSprite*(__thiscall*)(cocos2d::CCSprite*, cocos2d::CCMenu*, void(__stdcall* const)(void*))>(
				base + 0x18EE0
				)(sprite, target, callback);
			//fix stack before returning
			__asm add esp, 0x8
			return pRet;
		}
	};
	#pragma runtime_checks("s", restore)

	inline bool init() {
		//if the base address is valid, all other values should be valid.
		return base;
	}
}

#undef as
