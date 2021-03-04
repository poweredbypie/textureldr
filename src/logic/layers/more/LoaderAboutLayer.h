#ifndef __LOADERABOUTLAYER_H__
#define __LOADERABOUTLAYER_H__

#include "pch.h"

class LoaderAboutLayer : public gd::FLAlertLayer {
protected:
	virtual bool init();

	void onClose(cocos2d::CCObject*);
	void onAuthor(cocos2d::CCObject*);

public:
	static LoaderAboutLayer* create();
};

#endif