#ifndef __MENULAYER_H__
#define __MENULAYER_H__

#include <gd.h>

namespace gd {

	class FLAlertLayerProtocol;

	class MenuLayer : public cocos2d::CCLayer, public gd::FLAlertLayerProtocol {
	public:
		static MenuLayer* create() {
			return reinterpret_cast<MenuLayer* (__stdcall*)()>(
				base + 0x190550
				)();
		}
		static void fadeInMusic(const char* filename) {
			return reinterpret_cast<void(__stdcall*)(const char*)>(
				base + 0xC4BD0
				)(filename);
		}
	};
}

#endif