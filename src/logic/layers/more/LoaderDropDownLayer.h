#ifndef __LOADERDROPDOWNLAYER_H__
#define __LOADERDROPDOWNLAYER_H__

#include "pch.h"
#include "../LoaderLayer.h"
#include "LoaderOptionsLayer.h"
#include "LoaderAboutLayer.h"

class LoaderDropDownLayer : public gd::GJDropDownLayer {
protected:

protected:
	virtual void customSetup();
	virtual void exitLayer(cocos2d::CCObject*);
	void onOptions(cocos2d::CCObject*);
	void onAbout(cocos2d::CCObject*);

public:
	static LoaderDropDownLayer* create();

	friend class LoaderLayer;
};

#endif

