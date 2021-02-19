#pragma once

#include "pch.h"

#define as reinterpret_cast

namespace gd {
	inline auto base = as<char*>(GetModuleHandle(0));

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

	#pragma runtime_checks("s", off)
	class CCMenuItemSpriteExtra : public cocos2d::CCMenuItemSprite {
	public:
		static CCMenuItemSpriteExtra* create(cocos2d::CCSprite* sprite, cocos2d::CCMenu* target, void(__stdcall* const callback)(void*)) {
			auto pRet = as<CCMenuItemSpriteExtra*(__thiscall*)(cocos2d::CCSprite*, cocos2d::CCMenu*, void(__stdcall* const)(void*))>(
				base + 0x18EE0
				)(sprite, target, callback);
			//fix stack before returning
			__asm add esp, 0x8
			return pRet;
		}
	};

	class ButtonSprite : public cocos2d::CCSprite {
	public:
		/*PARAMS:
		* caption - the button's caption.
		* width - the width of the button. only used if absolute is TRUE.
		* absolute - whether or not to use provided width. if FALSE, game will auto-adjust.
		* font - font file for caption.
		* texture - texture file for button background.
		* height - height of button. put 0 for auto.
		* scale - scale of the caption.
		*/
		static ButtonSprite* create(const char* caption, int width, bool absolute, const char* font, const char* texture, float height, float scale) {
			//scale is passed in lower 4 bytes of xmm3
			__asm movss xmm3, scale
			//arg 3 is always 0. dunno why it's not optimized out as a param
			auto pRet = as<ButtonSprite* (__fastcall*)(const char*, int, int, bool, const char*, const char*, float)>(
				base + 0x137D0
				)(caption, width, 0, absolute, font, texture, height);
			//clean stack before returning
			__asm add esp, 0x14
			return pRet;
		}
	};
	#pragma runtime_checks("s", restore)

	class GJDropDownLayer : public cocos2d::CCNode {
	public:
		static GJDropDownLayer* create(const char* title) {
			GJDropDownLayer* pRet = as<decltype(pRet)>(new char[0x1C0]());
			if (pRet) {
				pRet->constructor();
				if (pRet->init(title)) {
					pRet->autorelease();
				}
				//no idea if this works correctly, destructor calls vtable function with stack cookie or something
				else pRet->destructor();
			}
			return pRet;
		}
		void constructor() {
			return as<void(__thiscall*)(GJDropDownLayer*)>(
				base + 0x038470
				)(this);
		}
		void destructor() {
			return as<void(__thiscall*)(GJDropDownLayer*, bool)>(
				base + 0x038560
				)(this, true);
		}
		bool init(const char* title) {
			return as<bool(__thiscall*)(GJDropDownLayer*, const char*)>(
				base + 0x113530
				)(this, title);
		}
		void showLayer(bool noTransition) {
			return as<void(__thiscall*)(GJDropDownLayer*, bool)>(
				base + 0x1139C0
				)(this, noTransition);
		}
	};

	inline bool init() {
		//if the base address is valid, all other values should be valid.
		log::info("Checking GeometryDash.exe base address...");
		return base;
	}
}

#undef as
