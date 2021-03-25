#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <gd.h>

namespace gd {
	class GameObject : public cocos2d::CCSprite {
	public:
		static GameObject* createWithFrame(const char* frame) {
			return reinterpret_cast<GameObject* (__fastcall*)(const char*)>(
				base + 0xCF8F0
				)(frame);
		}
	};
}

#endif