#include "pch.h"
#pragma once
#define PATH static inline const char*
#define FN_T using

namespace cocos {
	struct vec2 { float x, y; };
	extern std::vector<std::pair<void*&, const char*>> fcnPtrInfo;
	extern HMODULE hmodule;

	namespace director {
		PATH paGet{ "?sharedDirector@CCDirector@cocos2d@@SAPAV12@XZ" };
		PATH paReplaceScene{ "?replaceScene@CCDirector@cocos2d@@QAE_NPAVCCScene@2@@Z" };
		PATH paUpdateScale{ "?updateContentScale@CCDirector@cocos2d@@QAEXW4TextureQuality@2@@Z" };

		FN_T tGet = void* (__cdecl*)();
		FN_T tReplaceScene = bool(__thiscall*)(void* CCDirector, void* CCScene);
		FN_T tUpdateScale = void(__thiscall*)(void* CCDirector, int TextureQuality);

		extern tGet get;
		extern tReplaceScene replaceScene;
		extern tUpdateScale updateScale;

		vec2 getWinSize(void* CCDirector);
		void addTo();
	}
	namespace node {
		PATH paAddChild{ "?addChild@CCNode@cocos2d@@UAEXPAV12@@Z" };
		PATH paSetPos{ "?setPosition@CCNode@cocos2d@@UAEXMM@Z" };
		PATH paSetRot{ "?setRotation@CCNode@cocos2d@@UAEXM@Z" };

		FN_T tAddChild = void(__thiscall*)(void* CCNode, void* childNode);
		FN_T tSetPos = void(__thiscall*)(void* CCNode, float x, float y);
		FN_T tSetRot = void(__thiscall*)(void* CCNode, float rotation);

		extern tAddChild addChild;
		extern tSetPos setPos;
		extern tSetRot setRot;

		void addTo();
	}
	//---
	namespace scene {
		PATH paCreate{ "?create@CCScene@cocos2d@@SAPAV12@XZ" };

		FN_T tCreate = void* (__cdecl*)();

		extern tCreate create;

		void addTo();
	}
	namespace transition {
		PATH paCreate{ "?create@CCTransitionFade@cocos2d@@SAPAV12@MPAVCCScene@2@@Z" };

		FN_T tCreate = void* (__cdecl*)(float length, void* CCScene);

		extern tCreate create;

		void addTo();
	}
	//---
	namespace menu {
		PATH paCreate{ "?create@CCMenu@cocos2d@@SAPAV12@XZ" };

		FN_T tCreate = void* (__cdecl*)();

		extern tCreate create;

		void addTo();
	}
	namespace menuItem {
		PATH paCreateImg{ "?create@CCMenuItemImage@cocos2d@@SAPAV12@PBD00PAVCCObject@2@P832@AEX1@Z@Z" };
		PATH paCreateSpr{ "?create@CCMenuItemSprite@cocos2d@@SAPAV12@PAVCCNode@2@00PAVCCObject@2@P842@AEX1@Z@Z" };

		FN_T tCreateImg = void* (__cdecl*)(const char* normal, const char* selected, const char* disabled, void* target, void* selector);
		FN_T tCreateSpr = void* (__cdecl*)(void* normal, void* selected, void* disabled, void* target, void* selector);

		extern tCreateImg createImg;
		extern tCreateSpr createSpr;

		void addTo();
	}
	namespace label {
		PATH paCreate{ "?create@CCLabelBMFont@cocos2d@@SAPAV12@PBD0@Z" };
		PATH paSet{ "?setString@CCLabelBMFont@cocos2d@@UAEXPBD_N@Z" };

		FN_T tCreate = void* (__cdecl*)(const char* str, const char* fntFile);
		FN_T tSet = void(__thiscall*)(void* CCLabelBMFont, const char* str, bool update);

		extern tCreate create;
		extern tSet set;

		void addTo();
	}
	namespace sprite {
		PATH paCreate{ "?createWithSpriteFrameName@CCSprite@cocos2d@@SAPAV12@PBD@Z" };

		FN_T tCreate = void* (__cdecl*)(const char* txtrFile);

		extern tCreate create;

		void addTo();
	}
	namespace fileUtils {
		PATH paAddPath{ "?addSearchPath@CCFileUtils@cocos2d@@UAEXPBD@Z" };

		FN_T tAddPath = void(__thiscall*)(void* CCFileUtils, const char* path);

		extern tAddPath addPath;

		void addTo();
	}
	//--- unnecessary, was for executing code in cocos thread but unneeded after patching callbacks
	namespace scheduler {
		PATH paUpdate{ "?update@CCScheduler@cocos2d@@UAEXM@Z" };

		FN_T tUpdate = void(__thiscall*)(void* CCScheduler, float time);

		extern tUpdate update;

		void addTo();
	}

	void addFunctions();

	bool init();
}
