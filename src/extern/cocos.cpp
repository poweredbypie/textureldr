#include "pch.h"
#include "cocos.h"

namespace cocos {
	std::vector<std::pair<void*&, const char*>> fcnPtrInfo{};
	HMODULE hmodule{};

	namespace director {
		void* (__cdecl* get)();
		bool(__thiscall* replaceScene)(void* CCDirector, void* CCScene);
		void(__thiscall* updateScale)(void* CCDirector, int TextureQuality);

		vec2 getWinSize(void* CCDirector) {
			vec2 winSize;
			winSize.x = *(float*)((char*)CCDirector + 0xD4);
			winSize.y = *(float*)((char*)CCDirector + 0xD8);
			return winSize;
		}
		void addTo() {
			fcnPtrInfo.push_back({ (void*&)get, "?sharedDirector@CCDirector@cocos2d@@SAPAV12@XZ" });
			fcnPtrInfo.push_back({ (void*&)replaceScene, "?replaceScene@CCDirector@cocos2d@@QAE_NPAVCCScene@2@@Z" });
			fcnPtrInfo.push_back({ (void*&)updateScale, "?updateContentScale@CCDirector@cocos2d@@QAEXW4TextureQuality@2@@Z" });
		}
	}
	namespace node {
		void(__thiscall* addChild)(void* CCNode, void* childNode);
		void(__thiscall* setPos)(void* CCNode, float x, float y);
		void(__thiscall* setRot)(void* CCNode, float rotation);
		void(__thiscall* setScale)(void* CCNode, float scale);
		void(__thiscall* setVisible)(void* CCNode, bool visible);

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)addChild, "?addChild@CCNode@cocos2d@@UAEXPAV12@@Z" });
			fcnPtrInfo.push_back({ (void*&)setPos, "?setPosition@CCNode@cocos2d@@UAEXMM@Z" });
			fcnPtrInfo.push_back({ (void*&)setRot, "?setRotation@CCNode@cocos2d@@UAEXM@Z" });
			fcnPtrInfo.push_back({ (void*&)setScale, "?setScale@CCNode@cocos2d@@UAEXM@Z" });
			fcnPtrInfo.push_back({ (void*&)setVisible, "?setVisible@CCNode@cocos2d@@UAEX_N@Z" });
		}
	}
	//---
	namespace scene {
		void* (__cdecl* create)();

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)create, "?create@CCScene@cocos2d@@SAPAV12@XZ" });
		}
	}
	namespace transition {
		void* (__cdecl* create)(float length, void* CCScene);

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)create, "?create@CCTransitionFade@cocos2d@@SAPAV12@MPAVCCScene@2@@Z" });
		}
	}
	//---
	namespace menu {
		void* (__cdecl* create)();

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)create, "?create@CCMenu@cocos2d@@SAPAV12@XZ" });
		}
	}
	namespace menuItem {
		void* (__cdecl* createImg)(const char* normal, const char* selected, const char* disabled, void* target, void* selector);
		void* (__cdecl* createSpr)(void* normal, void* selected, void* disabled, void* target, void* selector);
		void(__thiscall* setEnabled)(void* CCMenuItem, bool value);

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)createImg, "?create@CCMenuItemImage@cocos2d@@SAPAV12@PBD00PAVCCObject@2@P832@AEX1@Z@Z" });
			fcnPtrInfo.push_back({ (void*&)createSpr, "?create@CCMenuItemSprite@cocos2d@@SAPAV12@PAVCCNode@2@00PAVCCObject@2@P842@AEX1@Z@Z" });
			fcnPtrInfo.push_back({ (void*&)setEnabled, "?setEnabled@CCMenuItem@cocos2d@@UAEX_N@Z" });
		}
	}
	namespace label {
		void* (__cdecl* create)(const char* str, const char* fntFile);
		void(__thiscall* set)(void* CCLabelBMFont, const char* str, bool update);

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)create, "?create@CCLabelBMFont@cocos2d@@SAPAV12@PBD0@Z" });
			fcnPtrInfo.push_back({ (void*&)set, "?setString@CCLabelBMFont@cocos2d@@UAEXPBD_N@Z" });
		}
	}
	namespace sprite {
		void* (__cdecl* create)(const char* txtrFile);

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)create, "?createWithSpriteFrameName@CCSprite@cocos2d@@SAPAV12@PBD@Z" });
		}
	}
	namespace fileUtils {
		void(__thiscall* addPath)(void* CCFileUtils, const char* path);

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)addPath, "?addSearchPath@CCFileUtils@cocos2d@@UAEXPBD@Z" });
		}
	}
	//---
	namespace xml {
		void(__thiscall* init)(void* XMLDocument, bool processEntities, int whitespaceMode);
		void(__thiscall* destruct)(void* XMLDocument);
		int(__thiscall* loadFile)(void* XMLDocument, const char* filename);
		int(__thiscall* saveFile)(void* XMLDocument, const char* filename, bool compact);
		void(__thiscall* deleteChildren)(void* XMLDocument);
		void* (__thiscall* newElement)(void* XMLDocument, const char* name);
		void* (__thiscall* newText)(void* XMLDocument, const char* text);
		void* (__thiscall* firstChildElement)(void* XMLNode, const char* name);
		void* (__thiscall* nextSiblingElement)(void* XMLNode, const char* name);
		void* (__thiscall* insertEndChild)(void* XMLNode, void* addThis);
		void* (__thiscall* insertFirstChild)(void* XMLNode, void* addThis);
		const char* (__thiscall* getText)(void* XMLElement);
		void(__thiscall* setValue)(void* XMLElement, const char* str, bool staticMem);
		void(__thiscall* setAttribute)(void* XMLElement, const char* name, int value);
		int(__thiscall* queryAttribute)(void* XMLElement, const char* name, int* value);

		void* create(bool processEntities, int whitespaceMode) {
			//dont want to deal with tinyxml dependencies, so this will do
			void* pRet = new char[0x170];
			if (pRet) {
				init(pRet, processEntities, whitespaceMode);
				return pRet;
			}
			else
				return 0;
		}
		void addTo() {
			fcnPtrInfo.push_back({ (void*&)init, "??0XMLDocument@tinyxml2@@QAE@_NW4Whitespace@1@@Z" });
			fcnPtrInfo.push_back({ (void*&)destruct, "??1XMLDocument@tinyxml2@@UAE@XZ" });
			fcnPtrInfo.push_back({ (void*&)loadFile, "?LoadFile@XMLDocument@tinyxml2@@QAE?AW4XMLError@2@PBD@Z" });
			fcnPtrInfo.push_back({ (void*&)saveFile, "?SaveFile@XMLDocument@tinyxml2@@QAE?AW4XMLError@2@PBD_N@Z" });
			fcnPtrInfo.push_back({ (void*&)deleteChildren, "?DeleteChildren@XMLNode@tinyxml2@@QAEXXZ" });
			fcnPtrInfo.push_back({ (void*&)newElement, "?NewElement@XMLDocument@tinyxml2@@QAEPAVXMLElement@2@PBD@Z" });
			fcnPtrInfo.push_back({ (void*&)newText, "?NewText@XMLDocument@tinyxml2@@QAEPAVXMLText@2@PBD@Z" });
			fcnPtrInfo.push_back({ (void*&)getText, "?GetText@XMLElement@tinyxml2@@QBEPBDXZ" });
			fcnPtrInfo.push_back({ (void*&)firstChildElement, "?FirstChildElement@XMLNode@tinyxml2@@QBEPBVXMLElement@2@PBD@Z" });
			fcnPtrInfo.push_back({ (void*&)nextSiblingElement, "?NextSiblingElement@XMLNode@tinyxml2@@QBEPBVXMLElement@2@PBD@Z" });
			fcnPtrInfo.push_back({ (void*&)insertEndChild, "?InsertEndChild@XMLNode@tinyxml2@@QAEPAV12@PAV12@@Z" });
			fcnPtrInfo.push_back({ (void*&)insertFirstChild, "?InsertFirstChild@XMLNode@tinyxml2@@QAEPAV12@PAV12@@Z" }); 
			fcnPtrInfo.push_back({ (void*&)setValue, "?SetValue@XMLNode@tinyxml2@@QAEXPBD_N@Z" });
			fcnPtrInfo.push_back({ (void*&)setAttribute, "?SetAttribute@XMLElement@tinyxml2@@QAEXPBDH@Z" });
			fcnPtrInfo.push_back({ (void*&)queryAttribute, "?QueryIntAttribute@XMLElement@tinyxml2@@QBE?AW4XMLError@2@PBDPAH@Z" });
		}
	}

	void addFunctions() {
		director::addTo();
		node::addTo();
		scene::addTo();
		transition::addTo();
		menu::addTo();
		menuItem::addTo();
		label::addTo();
		sprite::addTo();
		fileUtils::addTo();
		xml::addTo();
	}

	bool init() {
		addFunctions();

		if (hmodule = GetModuleHandle("libcocos2d.dll")) {
			for (std::pair<void*&, const char*>& pair : fcnPtrInfo) {
				pair.first = (void*)GetProcAddress(hmodule, pair.second);
				if (!pair.first)
					return false;
			}
			return true;
		}
		return false;
	}
}
