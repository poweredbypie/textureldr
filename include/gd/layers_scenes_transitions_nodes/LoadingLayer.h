#ifndef __LOADINGLAYER_H__
#define __LOADINGLAYER_H__

#include <gd.h>

namespace gd {
	class LoadingLayer : public cocos2d::CCLayer {
	public:
		void setWillFadeIn(bool willFadeIn) {
			*reinterpret_cast<bool*>(
				reinterpret_cast<char*>(this) + 0x138
				) = willFadeIn;
		}
	};
}

#endif