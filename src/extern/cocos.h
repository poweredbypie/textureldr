#pragma once

#include "pch.h"

//every function pointer is inlined since it's constant and defined in this header.
#define DEFINE_AND_ASSIGN(name, type, function)\
inline auto name = reinterpret_cast<type>(\
	GetProcAddress(reinterpret_cast<HMODULE>(base), function)\
);

namespace cocos {
	struct vec2 { float x, y; };
	inline char* base = reinterpret_cast<char*>(GetModuleHandle("libcocos2d.dll"));

	namespace director {
		DEFINE_AND_ASSIGN(get, void* (__cdecl const*)(), "?sharedDirector@CCDirector@cocos2d@@SAPAV12@XZ");
		DEFINE_AND_ASSIGN(replaceScene, bool(__thiscall const*)(void* CCDirector, void* CCScene), "?replaceScene@CCDirector@cocos2d@@QAE_NPAVCCScene@2@@Z");
		DEFINE_AND_ASSIGN(updateScale, void(__thiscall const*)(void* CCDirector, int TextureQuality), "?updateContentScale@CCDirector@cocos2d@@QAEXW4TextureQuality@2@@Z");

		inline const vec2 getWinSize(void* CCDirector) {
			return {
				*reinterpret_cast<float*>(reinterpret_cast<char*>(CCDirector) + 0xD4),
				*reinterpret_cast<float*>(reinterpret_cast<char*>(CCDirector) + 0xD8)
			};
		}
	}
	namespace node {
		DEFINE_AND_ASSIGN(addChild, void(__thiscall const*)(void* CCNode, void* childNode), "?addChild@CCNode@cocos2d@@UAEXPAV12@@Z");
		DEFINE_AND_ASSIGN(setPos, void(__thiscall const*)(void* CCNode, float x, float y), "?setPosition@CCNode@cocos2d@@UAEXMM@Z");
		DEFINE_AND_ASSIGN(setRot, void(__thiscall const*)(void* CCNode, float rotation), "?setRotation@CCNode@cocos2d@@UAEXM@Z");
		DEFINE_AND_ASSIGN(setScale, void(__thiscall const*)(void* CCNode, float scale), "?setScale@CCNode@cocos2d@@UAEXM@Z");
		DEFINE_AND_ASSIGN(setVisible, void(__thiscall const*)(void* CCNode, bool visible), "?setVisible@CCNode@cocos2d@@UAEX_N@Z");
	}
	//---
	namespace scene {
		DEFINE_AND_ASSIGN(create, void* (__cdecl const*)(), "?create@CCScene@cocos2d@@SAPAV12@XZ");
	}
	namespace transition {
		DEFINE_AND_ASSIGN(create, void* (__cdecl const*)(float length, void* CCScene), "?create@CCTransitionFade@cocos2d@@SAPAV12@MPAVCCScene@2@@Z");
	}
	//---
	namespace menu {
		DEFINE_AND_ASSIGN(create, void* (__cdecl const*)(), "?create@CCMenu@cocos2d@@SAPAV12@XZ");
	}
	namespace menuItem {
		DEFINE_AND_ASSIGN(
			createImg,
			void* (__cdecl const*)(const char* normal, const char* selected, const char* disabled, void* target, void* selector),
			"?create@CCMenuItemImage@cocos2d@@SAPAV12@PBD00PAVCCObject@2@P832@AEX1@Z@Z"
		);
		DEFINE_AND_ASSIGN(
			createSpr,
			void* (__cdecl const*)(void* normal, void* selected, void* disabled, void* target, void* selector),
			"?create@CCMenuItemSprite@cocos2d@@SAPAV12@PAVCCNode@2@00PAVCCObject@2@P842@AEX1@Z@Z"
		);
		DEFINE_AND_ASSIGN(setEnabled, void(__thiscall const*)(void* CCMenuItem, bool value), "?setEnabled@CCMenuItem@cocos2d@@UAEX_N@Z");
	}
	namespace label {
		DEFINE_AND_ASSIGN(create, void* (__cdecl const*)(const char* str, const char* fntFile), "?create@CCLabelBMFont@cocos2d@@SAPAV12@PBD0@Z");
		DEFINE_AND_ASSIGN(set, void(__thiscall const*)(void* CCLabelBMFont, const char* str, bool update), "?setString@CCLabelBMFont@cocos2d@@UAEXPBD_N@Z");
	}
	namespace sprite {
		DEFINE_AND_ASSIGN(create, void* (__cdecl const*)(const char* txtrFile), "?createWithSpriteFrameName@CCSprite@cocos2d@@SAPAV12@PBD@Z");
	}
	namespace fileUtils {
		DEFINE_AND_ASSIGN(addPath, void(__thiscall const*)(void* CCFileUtils, const char* path), "?addSearchPath@CCFileUtils@cocos2d@@UAEXPBD@Z");
	}
	//---
	namespace xml {
		DEFINE_AND_ASSIGN(init, void(__thiscall const*)(void* XMLDocument, bool processEntities, int whitespaceMode), "??0XMLDocument@tinyxml2@@QAE@_NW4Whitespace@1@@Z");
		DEFINE_AND_ASSIGN(loadFile, int(__thiscall const*)(void* XMLDocument, const char* filename), "?LoadFile@XMLDocument@tinyxml2@@QAE?AW4XMLError@2@PBD@Z");
		DEFINE_AND_ASSIGN(saveFile, int(__thiscall const*)(void* XMLDocument, const char* filename, bool compact), "?SaveFile@XMLDocument@tinyxml2@@QAE?AW4XMLError@2@PBD_N@Z");
		DEFINE_AND_ASSIGN(deleteChildren, void(__thiscall const*)(void* XMLDocument), "?DeleteChildren@XMLNode@tinyxml2@@QAEXXZ");
		DEFINE_AND_ASSIGN(newElement, void* (__thiscall const*)(void* XMLDocument, const char* name), "?NewElement@XMLDocument@tinyxml2@@QAEPAVXMLElement@2@PBD@Z");
		DEFINE_AND_ASSIGN(newText, void* (__thiscall const*)(void* XMLDocument, const char* text), "?NewText@XMLDocument@tinyxml2@@QAEPAVXMLText@2@PBD@Z");
		DEFINE_AND_ASSIGN(getText, const char* (__thiscall const*)(void* XMLElement), "?GetText@XMLElement@tinyxml2@@QBEPBDXZ");
		DEFINE_AND_ASSIGN(firstChildElement, void* (__thiscall const*)(void* XMLNode, const char* name), "?FirstChildElement@XMLNode@tinyxml2@@QBEPBVXMLElement@2@PBD@Z");
		DEFINE_AND_ASSIGN(nextSiblingElement, void* (__thiscall const*)(void* XMLNode, const char* name), "?NextSiblingElement@XMLNode@tinyxml2@@QBEPBVXMLElement@2@PBD@Z");
		DEFINE_AND_ASSIGN(insertEndChild, void* (__thiscall const*)(void* XMLNode, void* addThis), "?InsertEndChild@XMLNode@tinyxml2@@QAEPAV12@PAV12@@Z");
		DEFINE_AND_ASSIGN(insertFirstChild, void* (__thiscall const*)(void* XMLNode, void* addThis), "?InsertFirstChild@XMLNode@tinyxml2@@QAEPAV12@PAV12@@Z");
		DEFINE_AND_ASSIGN(setValue, void(__thiscall const* )(void* XMLElement, const char* str, bool staticMem), "?SetValue@XMLNode@tinyxml2@@QAEXPBD_N@Z");
		DEFINE_AND_ASSIGN(setAttribute, void(__thiscall const*)(void* XMLElement, const char* name, int value), "?SetAttribute@XMLElement@tinyxml2@@QAEXPBDH@Z");
		DEFINE_AND_ASSIGN(queryAttribute, int(__thiscall const*)(void* XMLElement, const char* name, int* value), "?QueryIntAttribute@XMLElement@tinyxml2@@QBE?AW4XMLError@2@PBDPAH@Z");

		inline void* create(bool processEntities, int whitespaceMode) {
			void* pRet = new char[0x170];
			if (pRet) {
				init(pRet, processEntities, whitespaceMode);
			}
			return pRet;
		}
	}

	inline bool init() {
		//if the base address is valid, all other values should be valid.
		return base;
	}
}

#undef DEFINE_AND_ASSIGN