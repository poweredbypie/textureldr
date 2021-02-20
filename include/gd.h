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
		void setQuality(cocos2d::TextureQuality quality) {
			*reinterpret_cast<cocos2d::TextureQuality*>(
				reinterpret_cast<char*>(this) + 0x2E4
				) = quality;
		}
	};

	class MenuLayer : public cocos2d::CCLayer {
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

	class LoadingLayer : public cocos2d::CCLayer {
	public:
		void setWillFadeIn(bool willFadeIn) {
			*reinterpret_cast<bool*>(
				reinterpret_cast<char*>(this) + 0x138
				) = willFadeIn;
		}
	};

	#pragma runtime_checks("s", off)
	class CCMenuItemSpriteExtra : public cocos2d::CCMenuItemSprite {
	public:
		static CCMenuItemSpriteExtra* create(cocos2d::CCSprite* sprite,
			cocos2d::CCMenu* target, void(__stdcall* const callback)(void*)) {
			auto pRet = as<CCMenuItemSpriteExtra*(__thiscall*)(cocos2d::CCSprite*,
				cocos2d::CCMenu*, void(__stdcall* const)(void*))>(
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
		static ButtonSprite* create(const char* caption, int width, bool absolute,
			const char* font, const char* texture, float height, float scale) {
			//scale is passed in lower 4 bytes of xmm3
			__asm movss xmm3, scale
			//arg 3 is always 0. dunno why it's not optimized out as a param
			auto pRet = as<ButtonSprite* (__fastcall*)(const char*,
				int, int, bool, const char*, const char*, float)>(
				base + 0x137D0
				)(caption, width, 0, absolute, font, texture, height);
			//clean stack before returning
			__asm add esp, 0x14
			return pRet;
		}
	};

	class FLAlertLayer : public cocos2d::CCLayer {
	public:
		/*FLAlertLayer supports colors of text for the caption. wrap desired text in "<cx></c>"
		* where x is the color desired. colors are:
		* r - red
		* l - lime
		* g - green
		* y - yellow
		* o - orange?
		* and more that i'm too lazy to find.
		*/
		static FLAlertLayer* create(cocos2d::CCObject* target, const char* title,
			const char* btn1, const char* btn2, std::string caption) {
			static_assert(sizeof(std::string) == 24, "std::string in debug mode does not work correctly with FLAlertLayer!");
			auto pRet = as<FLAlertLayer* (__fastcall*)(cocos2d::CCObject*, const char*,
				const char*, const char*, std::string)>(
					base + 0x22680
					)(target, title, btn1, btn2, caption);
			//clean stack.
			__asm add esp, 0x20
		}
		void show() {
			return as<void(__thiscall*)(FLAlertLayer*)>(
				base + 0x23560
				)(this);
		}
	};
	#pragma runtime_checks("s", restore)

	class GJDropDownLayer : public cocos2d::CCLayer {
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
