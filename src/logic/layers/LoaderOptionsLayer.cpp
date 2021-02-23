#include "pch.h"
#include "LoaderOptionsLayer.h"

using namespace cocos2d;

#if 0
void LoaderOptionsLayer::customSetup() {

}
#endif

LoaderOptionsLayer* LoaderOptionsLayer::create(const char* title) {
	LoaderOptionsLayer* pRet = new LoaderOptionsLayer();
	if (pRet) {
		pRet->constructor();
		if (pRet->init(title)) {
			pRet->autorelease();
		}
		else pRet->destructor();
	}
	return pRet;
}

