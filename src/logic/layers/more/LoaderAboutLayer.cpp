#include "pch.h"
#include "LoaderAboutLayer.h"

using namespace cocos2d;
using namespace extension;

bool LoaderAboutLayer::init() {
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	if (!this->initWithColor({ 0, 0, 0, 75 })) return false;
	m_pLayer = CCLayer::create();
	this->addChild(m_pLayer);

	auto bg = CCScale9Sprite::create("GJ_square01.png", { 0.0f, 0.0f, 80.0f, 80.0f });
	bg->setContentSize({ 360.0f, 260.0f });
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	m_pLayer->addChild(bg);

	m_pButtonMenu = CCMenu::create();
	m_pLayer->addChild(m_pButtonMenu);

	auto title = CCLabelBMFont::create("About", "goldFont.fnt");
	title->setScale(0.9f);
	title->setPosition(winSize.width / 2, winSize.height / 2 + 110.0f);
	m_pLayer->addChild(title);

	auto version = CCLabelBMFont::create("textureldr " VERSION, "bigFont.fnt");
	version->setScale(0.7f);
	version->setPosition(winSize.width / 2, winSize.height / 2 + 80.0f);
	m_pLayer->addChild(version);

	auto author = CCLabelBMFont::create("by PoweredByPie", "goldFont.fnt");
	author->setScale(0.5f);
	auto authorBtn = gd::CCMenuItemSpriteExtra::create(
		author,
		this,
		menu_selector(LoaderAboutLayer::onAuthor)
	);
	m_pButtonMenu->addChild(authorBtn);
	authorBtn->setPosition(0.0f, 60.0f);

	auto thanksTitle = CCLabelBMFont::create("thank you to", "bigFont.fnt");
	thanksTitle->setPosition(winSize.width / 2, winSize.height / 2 + 20.0f);
	thanksTitle->setScale(0.6f);
	thanksTitle->setColor({ 50, 255, 50 });
	m_pLayer->addChild(thanksTitle);

	auto thanks = CCMenu::create();
	thanks->addChild(CCLabelBMFont::create(
		"absol\n"
		"iad\n"
		"adaf\n"
		"mat\n"
		"tobuu\n",
		"goldFont.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentCenter));
	thanks->addChild(CCLabelBMFont::create(
		"andre\n"
		"cos8o\n"
		"figment\n"
		"hjfod\n"
		"fadey\n",
		"goldFont.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentCenter));
	//how do you center a ccmenu :(
	thanks->setPosition(winSize.width / 2 - 75.0f, winSize.height / 2 - 90.0f);
	thanks->setScale(0.7f);
	thanks->alignItemsHorizontallyWithPadding(30.0f);
	m_pLayer->addChild(thanks);

	auto closeBtn = gd::CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"),
		this,
		menu_selector(LoaderAboutLayer::onClose)
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

void LoaderAboutLayer::onClose(cocos2d::CCObject*) {
	this->setKeypadEnabled(false);
	this->removeFromParentAndCleanup(true);
}

void LoaderAboutLayer::onAuthor(cocos2d::CCObject*) {
	CCApplication::sharedApplication()->openURL("https://github.com/poweredbypie");
}

LoaderAboutLayer* LoaderAboutLayer::create() {
	LoaderAboutLayer* pRet = new LoaderAboutLayer();
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}