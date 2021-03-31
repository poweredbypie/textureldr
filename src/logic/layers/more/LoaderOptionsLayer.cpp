#include "pch.h"
#include "LoaderOptionsLayer.h"

using namespace cocos2d;
using namespace extension;

bool LoaderOptionsLayer::init() {
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	if (!this->initWithColor({ 0, 0, 0, 75 })) return false;

	auto bg = CCScale9Sprite::create("GJ_square01.png", { 0.0f, 0.0f, 80.0f, 80.0f });
	bg->setContentSize({ 360.0f, 260.0f });
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	m_pLayer->addChild(bg);

	m_pButtonMenu = CCMenu::create();
	m_pLayer->addChild(m_pButtonMenu);

	auto title = CCLabelBMFont::create("Options", "goldFont.fnt");
	title->setScale(0.9f);
	title->setPosition(winSize.width / 2, winSize.height / 2 + 110.0f);
	m_pLayer->addChild(title);

	auto loaderManager = LoaderManager::sharedState();

	this->createToggler("Reload SFX",
		loaderManager->m_bReloadSFX,
		"Reload .ogg files on Apply.\n"
		"Increases performance if toggled off.");
	this->createToggler("Reload Music",
		loaderManager->m_bReloadMusic,
		"Reload menuLoop.mp3 on Apply.\n"
		"May slightly increase performance if toggled off.");
	this->createToggler("Check .plists",
		loaderManager->m_bCheckPlists,
		"Check .plist / .fnt files on Apply.\n"
		"Will warn user if plists will break pack combination.\n"
		"Increases performance if toggled off.");
	this->createToggler("Check Quality",
		loaderManager->m_bCheckQuality,
		"Check quality of applied packs on Apply.\n"
		"Will warn user if current quality doesn't line up with packs.\n"
		"Increases performance if toggled off.");
	this->createToggler("Enhance Quality",
		loaderManager->m_bEnhanceQuality,
		"Enhance texture quality.\n"
		"Changes CCTexture2D pixel format to use 32-bit color.\n"
		"Seems to increase performance if toggled on. (unsure)");

	auto closeBtn = gd::CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"),
		this,
		menu_selector(LoaderOptionsLayer::onClose)
	);
	m_pButtonMenu->addChild(closeBtn);
	closeBtn->setPosition(
		closeBtn->convertToNodeSpace({ (winSize.width / 2) - 198.0f, (winSize.height / 2) + 102.0f })
	);
	closeBtn->setSizeMult(1.5f);

	this->setKeypadEnabled(true);
	this->setTouchEnabled(true);
	return true;
}

LoaderOptionsLayer::LoaderOptionsLayer() {
	m_fLastX = -140.0f;
	m_fLastY = 65.0f;

	m_pLayer = CCLayer::create();
	this->addChild(m_pLayer);
}

void LoaderOptionsLayer::onClose(cocos2d::CCObject*) {
	this->setKeypadEnabled(false);
	this->removeFromParentAndCleanup(true);
}

void LoaderOptionsLayer::createToggler(const char* title, bool& toggle, const char* description) {
	
	if (m_fLastY <= -100.0f) {
		m_fLastY = 65.0f;
		m_fLastX += 170.0f;
	}
	if (m_fLastX >= 40.0f) {
		//handle this when u have >10 settings
	}

	auto offSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
	offSprite->setScale(0.75f);
	auto onSprite = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
	onSprite->setScale(0.75f);
	auto toggler = gd::CCMenuItemToggler::create(
		offSprite,
		onSprite,
		this,
		menu_selector(LoaderOptionsLayer::toggleToggler)
	);
	toggler->setUserData(&toggle);
	//idk why this doesnt work
	toggler->toggle(toggle);
	toggler->setPosition(0.0f + m_fLastX, 0.0f + m_fLastY);
	m_pButtonMenu->addChild(toggler);

	if (description) {
		auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
		infoSprite->setScale(0.5f);
		auto info = gd::CCMenuItemSpriteExtra::create(
			infoSprite,
			this,
			menu_selector(LoaderOptionsLayer::viewTogglerDescription)
		);
		//i know this isn't gonna be modified anyway
		info->setUserData(const_cast<char*>(description));
		info->setPosition(-20.0f + m_fLastX, 20.0f + m_fLastY);
		m_pButtonMenu->addChild(info);
	}

	auto label = CCLabelBMFont::create(title, "bigFont.fnt");
	label->setPosition(75.0f + m_fLastX, 0.0f + m_fLastY);
	label->limitLabelWidth(110.0f, 15.0f, 0.0f);
	m_pButtonMenu->addChild(label);

	m_fLastY -= 40.0f;
}

void LoaderOptionsLayer::toggleToggler(cocos2d::CCObject* toggler) {
	bool* toggle = static_cast<bool*>(static_cast<CCNode*>(toggler)->getUserData());
	*toggle = !*toggle;
}

void LoaderOptionsLayer::viewTogglerDescription(cocos2d::CCObject* info) {
	gd::FLAlertLayer::create(nullptr, "Info", "OK", 0, 425.0f,
		static_cast<const char*>(static_cast<CCNode*>(info)->getUserData()))->show();
}

LoaderOptionsLayer* LoaderOptionsLayer::create() {
	LoaderOptionsLayer* pRet = new LoaderOptionsLayer();
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}