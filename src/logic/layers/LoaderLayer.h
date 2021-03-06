#ifndef __LOADERLAYER_H__
#define __LOADERLAYER_H__

#include "pch.h"
#include "more/LoaderDropDownLayer.h"
#include "../nodes/List.h"
#include "../nodes/LoaderManager.h"

class LoaderLayer : public cocos2d::CCLayer, gd::FLAlertLayerProtocol {
protected:
	HorizontalList* m_plQuality;
	VerticalList* m_plAll;
	VerticalList* m_plApplied;

protected:
	virtual bool init();

	virtual void update(float dt);

	virtual void keyDown(cocos2d::enumKeyCodes key);
	virtual void scrollWheel(float x, float y);

	inline VerticalList* listForMousePos() {
		auto gl = cocos2d::CCEGLView::sharedOpenGLView();
		return gl->getMousePosition().x > gl->getFrameSize().width / 2
			? m_plApplied : m_plAll;
	}

	virtual void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2);

	void onApply(cocos2d::CCObject*);
	void onDropDown(cocos2d::CCObject*);
	void onExit(cocos2d::CCObject*);
	void onRefresh(cocos2d::CCObject*);
	void onFolder(cocos2d::CCObject*);

	void reloadAll();

public:
	static LoaderLayer* create();
	static void __fastcall scene(gd::MenuLayer* This, void*, cocos2d::CCObject* btn);

	friend class LoaderDropDownLayer;
};

#endif

