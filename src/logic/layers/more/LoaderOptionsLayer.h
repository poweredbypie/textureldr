#ifndef __LOADEROPTIONSLAYER_H__
#define __LOADEROPTIONSLAYER_H__

#include "pch.h"
#include "../../nodes/LoaderManager.h"

class LoaderOptionsLayer : public gd::FLAlertLayer {
protected:
	float m_fLastX;
	float m_fLastY;

protected:
	virtual bool init();

	LoaderOptionsLayer();

	void onClose(cocos2d::CCObject*);

	void createToggler(const char* title, bool& toggle, const char* description);
	void toggleToggler(cocos2d::CCObject* toggler);
	void viewTogglerDescription(cocos2d::CCObject* info);

public:
	static LoaderOptionsLayer* create();
};

#endif