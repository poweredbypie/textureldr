#ifndef __LOADERDROPDOWNLAYER_H__
#define __LOADERDROPDOWNLAYER_H__

#include "pch.h"
#include "LoaderOptionsLayer.h"
#include "LoaderAboutLayer.h"

class LoaderDropDownLayer : public gd::GJDropDownLayer {
protected:

protected:
	virtual void customSetup();
	void onOptions(cocos2d::CCObject*);
	void onAbout(cocos2d::CCObject*);

public:
	static LoaderDropDownLayer* create();
};

#endif

