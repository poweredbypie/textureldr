#include "pch.h"
#include "cocos.h"

namespace cocos {
	std::vector<std::pair<void*&, const char*>> fcnPtrInfo{};
	HMODULE hmodule{};

	namespace director {
		tGet get;
		tReplaceScene replaceScene;
		tUpdateScale updateScale;	

		vec2 getWinSize(void* CCDirector) {
			vec2 winSize;
			winSize.x = *(float*)((char*)CCDirector + 0xD4);
			winSize.y = *(float*)((char*)CCDirector + 0xD8);
			return winSize;
		}
		void addTo() {
			fcnPtrInfo.push_back({ (void*&)get, paGet });
			fcnPtrInfo.push_back({ (void*&)replaceScene, paReplaceScene });
			fcnPtrInfo.push_back({ (void*&)updateScale, paUpdateScale });
		}
	}
	namespace node {
		tAddChild addChild;
		tSetPos setPos;
		tSetRot setRot;
		tSetScale setScale;
		tSetVisible setVisible;

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)addChild, paAddChild });
			fcnPtrInfo.push_back({ (void*&)setPos, paSetPos });
			fcnPtrInfo.push_back({ (void*&)setRot, paSetRot });
			fcnPtrInfo.push_back({ (void*&)setScale, paSetScale });
			fcnPtrInfo.push_back({ (void*&)setVisible, paSetVisible });
		}
	}
	//---
	namespace scene {
		tCreate create;

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)create, paCreate });
		}
	}
	namespace transition {
		tCreate create;

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)create, paCreate });
		}
	}
	//---
	namespace menu {
		tCreate create;

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)create, paCreate });
		}
	}
	namespace menuItem {
		tCreateImg createImg;
		tCreateSpr createSpr;
		tSetEnabled setEnabled;

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)createImg, paCreateImg });
			fcnPtrInfo.push_back({ (void*&)createSpr, paCreateSpr });
			fcnPtrInfo.push_back({ (void*&)setEnabled, paSetEnabled });
		}
	}
	namespace label {
		tCreate create;
		tSet set;

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)create, paCreate });
			fcnPtrInfo.push_back({ (void*&)set, paSet });
		}
	}
	namespace sprite {
		tCreate create;

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)create, paCreate });
		}
	}
	namespace fileUtils {
		tAddPath addPath;

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)addPath, paAddPath});
		}
	}
	//---
	namespace scheduler {
		tUpdate update;

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)update, paUpdate });
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
		scheduler::addTo();
	}

	bool init() {
		addFunctions();

		hmodule = GetModuleHandle("libcocos2d.dll");

		if (hmodule) {
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
