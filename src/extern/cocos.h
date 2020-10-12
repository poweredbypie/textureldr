#pragma once

#include "pch.h"

namespace cocos {
	struct vec2 { float x, y; };
	struct color3b { unsigned char r, g, b; };
	extern std::vector<std::pair<void*&, const char*>> fcnPtrInfo;
	extern HMODULE hmodule;

	namespace director {
		extern void* (__cdecl* get)();
		extern bool(__thiscall* replaceScene)(void* CCDirector, void* CCScene);
		extern void(__thiscall* updateScale)(void* CCDirector, int TextureQuality);

		vec2 getWinSize(void* CCDirector);
	}
	namespace node {
		extern void(__thiscall* addChild)(void* CCNode, void* childNode);
		extern void(__thiscall* setPos)(void* CCNode, float x, float y);
		extern void(__thiscall* setRot)(void* CCNode, float rotation);
		extern void(__thiscall* setScale)(void* CCNode, float scale);
		extern void(__thiscall* setVisible)(void* CCNode, bool visible);
	}
	//---
	namespace scene {
		extern void* (__cdecl* create)();
	}
	namespace transition {
		extern void* (__cdecl* create)(float length, void* CCScene);
	}
	//---
	namespace menu {
		extern void* (__cdecl* create)();
	}
	namespace menuItem {
		extern void* (__cdecl* createImg)(const char* normal, const char* selected, const char* disabled, void* target, void* selector);
		extern void* (__cdecl* createSpr)(void* normal, void* selected, void* disabled, void* target, void* selector);
		extern void(__thiscall* setEnabled)(void* CCMenuItem, bool value);
	}
	namespace label {
		extern void* (__cdecl* create)(const char* str, const char* fntFile);
		extern void(__thiscall* set)(void* CCLabelBMFont, const char* str, bool update);
	}
	namespace sprite {
		extern void* (__cdecl* create)(const char* txtrFile);
	}
	namespace fileUtils {
		extern void(__thiscall* addPath)(void* CCFileUtils, const char* path);
	}
	//---
	namespace xml {
		extern void(__thiscall* destruct)(void* XMLDocument);
		extern int(__thiscall* loadFile)(void* XMLDocument, const char* filename);
		extern int(__thiscall* saveFile)(void* XMLDocument, const char* filename, bool compact);
		extern void(__thiscall* deleteChildren)(void* XMLDocument);
		extern void* (__thiscall* newElement)(void* XMLDocument, const char* name);
		extern void* (__thiscall* newText)(void* XMLDocument, const char* text);
		extern void* (__thiscall* firstChildElement)(void* XMLNode, const char* name);
		extern void* (__thiscall* nextSiblingElement)(void* XMLNode, const char* name);
		extern void* (__thiscall* insertEndChild)(void* XMLNode, void* addThis);
		extern void* (__thiscall* insertFirstChild)(void* XMLNode, void* addThis);
		extern const char* (__thiscall* getText)(void* XMLElement);
		extern void(__thiscall* setValue)(void* XMLElement, const char* str, bool staticMem);
		extern void(__thiscall* setAttribute)(void* XMLElement, const char* name, int value);
		extern int(__thiscall* queryAttribute)(void* XMLElement, const char* name, int* value);

		void* create(bool processEntities, int whitespaceMode);
	}

	bool init();
}