#include "pch.h"
#include "LoaderDropDownLayer.h"

using namespace cocos2d;

void LoaderDropDownLayer::customSetup() {
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto menu = CCMenu::create();
	m_pLayer->addChild(menu);

	auto optionsBtn = gd::CCMenuItemSpriteExtra::create(
		gd::ButtonSprite::create("Options", 150, true, "goldFont.fnt", "GJ_button_01.png", 0.0f, 1.0f),
		this,
		menu_selector(LoaderDropDownLayer::onOptions)
	);
	menu->addChild(optionsBtn);
	auto aboutBtn = gd::CCMenuItemSpriteExtra::create(
		gd::ButtonSprite::create("About", 150, true, "goldFont.fnt", "GJ_button_01.png", 0.0f, 1.0f),
		this,
		menu_selector(LoaderDropDownLayer::onAbout)
	);
	menu->addChild(aboutBtn);

	menu->alignItemsVerticallyWithPadding(20.0f);
}

void LoaderDropDownLayer::exitLayer(cocos2d::CCObject*) {
	auto parent = static_cast<LoaderLayer*>(this->getParent());
	parent->scheduleUpdate();
	parent->setMouseEnabled(true);

	this->setKeypadEnabled(false);
	CCDirector::sharedDirector()->getTouchDispatcher()->setForcePrio(false);
	this->disableUI();
	this->hideLayer(false);
}

void LoaderDropDownLayer::onOptions(cocos2d::CCObject*) {
	LoaderOptionsLayer::create()->show();
}

void LoaderDropDownLayer::onAbout(cocos2d::CCObject*) {
	LoaderAboutLayer::create()->show();
}

LoaderDropDownLayer* LoaderDropDownLayer::create() {
	LoaderDropDownLayer* pRet = new LoaderDropDownLayer();
	if (pRet && pRet->init("More", 220.0f)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

