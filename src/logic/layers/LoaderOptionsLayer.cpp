#include "pch.h"
#include "LoaderOptionsLayer.h"

using namespace cocos2d;

void LoaderOptionsLayer::customSetup() {
	auto testBtn = gd::CCMenuItemSpriteExtra::create(
		gd::ButtonSprite::create("hey", 0, false, "goldFont.fnt", "GJ_button_01.png", 0.0f, 1.0f),
		this,
		nullptr
	);
	m_pLayer->addChild(CCMenu::createWithItem(testBtn));
}

LoaderOptionsLayer* LoaderOptionsLayer::create() {
	LoaderOptionsLayer* pRet = new LoaderOptionsLayer();
	if (pRet && pRet->init("Options", 220.0f)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

