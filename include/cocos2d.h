#pragma once

#include "pch.h"

#define FIND_EXPORT(name)\
GetProcAddress(base, name)

//reinterpret_cast is too long im lazy
#define as reinterpret_cast

namespace cocos2d {
	inline auto base = GetModuleHandle("libcocos2d.dll");

	typedef struct {
		float width, height;
	} CCSize;

	typedef enum {
		LOW = 1,
		MEDIUM,
		HIGH
	} TextureQuality;

	class CCNode {
	public:
		void addChild(CCNode* childNode) {
			return as<void(__thiscall*)(CCNode*, CCNode*)>(
				FIND_EXPORT("?addChild@CCNode@cocos2d@@UAEXPAV12@@Z")
				)(this, childNode);
		}
		void setPosition(float x, float y) {
			return as<void(__thiscall*)(CCNode*, float, float)>(
				FIND_EXPORT("?setPosition@CCNode@cocos2d@@UAEXMM@Z")
				)(this, x, y);
		}
		void setRotation(float rotation) {
			return as<void(__thiscall*)(CCNode*, float)>(
				FIND_EXPORT("?setRotation@CCNode@cocos2d@@UAEXM@Z")
				)(this, rotation);
		}
		void setScale(float scale) {
			return as<void(__thiscall*)(CCNode*, float)>(
				FIND_EXPORT("?setScale@CCNode@cocos2d@@UAEXM@Z")
				)(this, scale);
		}
		void setVisible(bool visible) {
			return as<void(__thiscall*)(CCNode*, bool)>(
				FIND_EXPORT("?setVisible@CCNode@cocos2d@@UAEX_N@Z")
				)(this, visible);
		}
	};

	class CCScene : public CCNode {
	public:
		static CCScene* create() {
			return as<CCScene* (__cdecl*)()>(
				FIND_EXPORT("?create@CCScene@cocos2d@@SAPAV12@XZ")
				)();
		}
	};

	class CCTransitionFade : public CCScene {
	public:
		static CCTransitionFade* create(float length, CCScene* scene) {
			return as<CCTransitionFade* (__cdecl*)(float, CCScene*)>(
				FIND_EXPORT("?create@CCTransitionFade@cocos2d@@SAPAV12@MPAVCCScene@2@@Z")
				)(length, scene);
		}
	};

	class CCMenu : public CCNode {
	public:
		static CCMenu* create() {
			return as<CCMenu* (__cdecl*)()>(
				FIND_EXPORT("?create@CCMenu@cocos2d@@SAPAV12@XZ")
				)();
		}
	};

	class CCMenuItem : public CCNode {
	public:
		void setEnabled(bool value) {
			return as<void(__thiscall*)(CCMenuItem*, bool)>(
				FIND_EXPORT("?setEnabled@CCMenuItem@cocos2d@@UAEX_N@Z")
				)(this, value);
		}
	};

	class CCLabelBMFont : public CCNode {
	public:
		static CCLabelBMFont* create(const char* str, const char* fntFile) {
			return as<CCLabelBMFont* (__cdecl*)(const char*, const char*)>(
				FIND_EXPORT("?create@CCLabelBMFont@cocos2d@@SAPAV12@PBD0@Z")
				)(str, fntFile);
		}
		void setString(const char* str, bool update) {
			return as<void(__thiscall*)(CCLabelBMFont*, const char*, bool)>(
				FIND_EXPORT("?setString@CCLabelBMFont@cocos2d@@UAEXPBD_N@Z")
				)(this, str, update);
		}
	};

	class CCSprite : public CCNode {
	public:
		static CCSprite* createWithSpriteFrameName(const char* txtrFile) {
			return as<CCSprite* (__cdecl*)(const char*)>(
				FIND_EXPORT("?createWithSpriteFrameName@CCSprite@cocos2d@@SAPAV12@PBD@Z")
				)(txtrFile);
		}
		static CCSprite* create(const char* txtrFile) {
			return as<CCSprite* (__cdecl*)(const char*)>(
				FIND_EXPORT("?create@CCSprite@cocos2d@@SAPAV12@PBD@Z")
				)(txtrFile);
		}
	};

	class CCFileUtils {
	public:
		void addSearchPath(const char* path) {
			return as<void(__thiscall*)(CCFileUtils*, const char*)>(
				FIND_EXPORT("?addSearchPath@CCFileUtils@cocos2d@@UAEXPBD@Z")
				)(this, path);
		}
	};

	namespace tinyxml2 {
		enum XMLError {
			XML_NO_ERROR = 0,
			XML_SUCCESS = 0,

			XML_NO_ATTRIBUTE,
			XML_WRONG_ATTRIBUTE_TYPE,

			XML_ERROR_FILE_NOT_FOUND,
			XML_ERROR_FILE_COULD_NOT_BE_OPENED,
			XML_ERROR_FILE_READ_ERROR,
			XML_ERROR_ELEMENT_MISMATCH,
			XML_ERROR_PARSING_ELEMENT,
			XML_ERROR_PARSING_ATTRIBUTE,
			XML_ERROR_IDENTIFYING_TAG,
			XML_ERROR_PARSING_TEXT,
			XML_ERROR_PARSING_CDATA,
			XML_ERROR_PARSING_COMMENT,
			XML_ERROR_PARSING_DECLARATION,
			XML_ERROR_PARSING_UNKNOWN,
			XML_ERROR_EMPTY_DOCUMENT,
			XML_ERROR_MISMATCHED_ELEMENT,
			XML_ERROR_PARSING,

			XML_CAN_NOT_CONVERT_TEXT,
			XML_NO_TEXT_NODE
		};

		class XMLElement;
		class XMLText;

		class XMLNode {
		public:
			void DeleteChildren() {
				return as<void(__thiscall*)(XMLNode*)>(
					FIND_EXPORT("?DeleteChildren@XMLNode@tinyxml2@@QAEXXZ")
					)(this);
			}
			XMLElement* FirstChildElement(const char* name) {
				return as<XMLElement* (__thiscall*)(XMLNode*, const char*)>(
					FIND_EXPORT("?FirstChildElement@XMLNode@tinyxml2@@QBEPBVXMLElement@2@PBD@Z")
					)(this, name);
			}
			XMLElement* NextSiblingElement(const char* name) {
				return as<XMLElement* (__thiscall*)(XMLNode*, const char*)>(
					FIND_EXPORT("?NextSiblingElement@XMLNode@tinyxml2@@QBEPBVXMLElement@2@PBD@Z")
					)(this, name);
			}
			XMLNode* InsertEndChild(XMLNode* addThis) {
				return as<XMLNode* (__thiscall*)(XMLNode*, XMLNode*)>(
					FIND_EXPORT("?InsertEndChild@XMLNode@tinyxml2@@QAEPAV12@PAV12@@Z")
					)(this, addThis);
			}
			XMLNode* InsertFirstChild(XMLNode* addThis) {
				return as<XMLNode* (__thiscall*)(XMLNode*, XMLNode*)>(
					FIND_EXPORT("?InsertFirstChild@XMLNode@tinyxml2@@QAEPAV12@PAV12@@Z")
					)(this, addThis);
			}
			void SetValue(const char* str, bool staticMem) {
				return as<void(__thiscall*)(XMLNode*, const char*, bool)>(
					FIND_EXPORT("?SetValue@XMLNode@tinyxml2@@QAEXPBD_N@Z")
					)(this, str, staticMem);
			}
		};

		class XMLDocument : public XMLNode {
		public:
			static XMLDocument* create(bool processEntities, int whitespaceMode) {
				XMLDocument* pRet = as<decltype(pRet)>(new char[0x170]);
				if (pRet) {
					pRet->init(processEntities, whitespaceMode);
				}
				return pRet;
			}

			void init(bool processEntities, int whitespaceMode) {
				return as<void(__thiscall*)(XMLDocument*, bool, int)>(
					FIND_EXPORT("??0XMLDocument@tinyxml2@@QAE@_NW4Whitespace@1@@Z")
					)(this, processEntities, whitespaceMode);
			}
			XMLError LoadFile(const char* filename) {
				return as<XMLError(__thiscall*)(XMLDocument*, const char*)>(
					FIND_EXPORT("?LoadFile@XMLDocument@tinyxml2@@QAE?AW4XMLError@2@PBD@Z")
					)(this, filename);
			}
			XMLError SaveFile(const char* filename, bool compact) {
				return as<XMLError(__thiscall*)(XMLDocument*, const char*, bool)>(
					FIND_EXPORT("?SaveFile@XMLDocument@tinyxml2@@QAE?AW4XMLError@2@PBD_N@Z")
					)(this, filename, compact);
			}
			XMLElement* NewElement(const char* name) {
				return as<XMLElement* (__thiscall*)(XMLDocument*, const char*)>(
					FIND_EXPORT("?NewElement@XMLDocument@tinyxml2@@QAEPAVXMLElement@2@PBD@Z")
					)(this, name);
			}
			XMLText* NewText(const char* text) {
				return as<XMLText* (__thiscall*)(XMLDocument*, const char*)>(
					FIND_EXPORT("?NewText@XMLDocument@tinyxml2@@QAEPAVXMLText@2@PBD@Z")
					)(this, text);
			}
		};

		class XMLElement : public XMLNode {
		public:
			const char* GetText() {
				return as<const char* (__thiscall*)(XMLElement*)>(
					FIND_EXPORT("?GetText@XMLElement@tinyxml2@@QBEPBDXZ")
					)(this);
			}
			void SetAttribute(const char* name, int value) {
				return as<void(__thiscall*)(XMLElement*, const char*, int)>(
					FIND_EXPORT("?SetAttribute@XMLElement@tinyxml2@@QAEXPBDH@Z")
					)(this, name, value);
			}
			XMLError QueryIntAttribute(const char* name, int* value) {
				return as<XMLError(__thiscall*)(XMLElement*, const char*, int*)>(
					FIND_EXPORT("?QueryIntAttribute@XMLElement@tinyxml2@@QBE?AW4XMLError@2@PBDPAH@Z")
					)(this, name, value);
			}
		};

		class XMLText : public XMLNode {
			//lol idc
		};
	}

	class CCDirector : public CCNode {
	public:
		static CCDirector* sharedDirector() {
			return as<CCDirector* (__cdecl*)()>(
				FIND_EXPORT("?sharedDirector@CCDirector@cocos2d@@SAPAV12@XZ")
				)();
		}
		bool replaceScene(CCScene* scene) {
			return as<bool(__thiscall*)(CCDirector*, CCScene*)>(
				FIND_EXPORT("?replaceScene@CCDirector@cocos2d@@QAE_NPAVCCScene@2@@Z")
				)(this, scene);
		}
		void updateContentScale(TextureQuality quality) {
			return as<void(__thiscall*)(CCDirector*, TextureQuality)>(
				FIND_EXPORT("?updateContentScale@CCDirector@cocos2d@@QAEXW4TextureQuality@2@@Z")
				)(this, quality);
		}
		const CCSize getWinSize() {
			return { *as<float*>(as<char*>(this) + 0xD4), *as<float*>(as<char*>(this) + 0xD8) };
		}
	};

	inline bool init() {
		return base;
	}
}

#undef as
#undef FIND_EXPORT