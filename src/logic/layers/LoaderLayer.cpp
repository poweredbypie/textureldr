#include "pch.h"
#include "LoaderLayer.h"

using namespace cocos2d;

enum {
	kPlistWarningLayer,
	kQualityWarningLayer
};

bool LoaderLayer::init() {
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto bg = CCSprite::create("GJ_gradientBG.png");
	auto bgSize = bg->getTextureRect().size;
	bg->setAnchorPoint({ 0.0f, 0.0f });
	bg->setScaleX((winSize.width + 10.0f) / bgSize.width);
	bg->setScaleY((winSize.height + 10.0f) / bgSize.height);
	bg->setPosition({ -5.0f, -5.0f });
	bg->setColor({ 0, 102, 255 });
	this->addChild(bg);

	auto bottomLeft = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
	auto cornerSize = bottomLeft->getTextureRect().size;
	bottomLeft->setPosition({ cornerSize.width / 2, cornerSize.height / 2 });
	auto bottomRight = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
	bottomRight->setFlipX(true);
	bottomRight->setPosition({ winSize.width - cornerSize.width / 2, cornerSize.height / 2 });
	auto topLeft = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
	topLeft->setFlipY(true);
	topLeft->setPosition({ cornerSize.width / 2, winSize.height - cornerSize.height / 2 });
	auto topRight = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
	topRight->setFlipX(true);
	topRight->setFlipY(true);
	topRight->setPosition({ winSize.width - cornerSize.width / 2, winSize.height - cornerSize.height / 2 });
	this->addChild(bottomLeft);
	this->addChild(bottomRight);
	this->addChild(topLeft);
	this->addChild(topRight);

	auto buttons = CCMenu::create();
	this->addChild(buttons);

	auto applyBtn = gd::CCMenuItemSpriteExtra::create(
		gd::ButtonSprite::create("Apply", 0, false, "goldFont.fnt", "GJ_button_01.png", 0.0f, 1.0f),
		this,
		menu_selector(LoaderLayer::onApply)
	);
	applyBtn->setPosition(95.0f, -135.0f);
	buttons->addChild(applyBtn);

	auto backBtn = gd::CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
		this,
		menu_selector(LoaderLayer::onExit)
	);
	backBtn->setPosition(-winSize.width / 2 + 25.0f, winSize.height / 2 - 25.0f);
	buttons->addChild(backBtn);

	auto reloadSprite = CCSprite::createWithSpriteFrameName("GJ_replayBtn_001.png");
	reloadSprite->setScale(0.85f);
	auto reloadBtn = gd::CCMenuItemSpriteExtra::create(
		reloadSprite,
		this,
		menu_selector(LoaderLayer::onRefresh)
	);
	reloadBtn->setPosition(winSize.width / 2 - 35.0f, -winSize.height / 2 + 35.0f);
	reloadBtn->setSizeMult(1.5f);
	buttons->addChild(reloadBtn);

	auto dropDownSprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png");
	dropDownSprite->setScale(1.3f);
	auto dropDownBtn = gd::CCMenuItemSpriteExtra::create(
		dropDownSprite,
		this,
		menu_selector(LoaderLayer::onDropDown)
	);
	dropDownBtn->setPosition(-winSize.width / 2 + 35.0f, -winSize.height / 2 + 35.0f);
	dropDownBtn->setSizeMult(1.5f);
	buttons->addChild(dropDownBtn);

	auto folderSprite = CCSprite::createWithSpriteFrameName("GJ_duplicateBtn_001.png");
	folderSprite->setScale(1.1f);
	auto folderBtn = gd::CCMenuItemSpriteExtra::create(
		folderSprite,
		this,
		menu_selector(LoaderLayer::onFolder)
	);
	folderBtn->setPosition(winSize.width / 2 - 35.0f, winSize.height / 2 - 35.0f);
	folderBtn->setSizeMult(1.5f);
	buttons->addChild(folderBtn);

	auto loaderManager = LoaderManager::sharedState();

	m_plQuality = HorizontalList::create(loaderManager->m_dQuality);
	m_plQuality->setPosition(-95.0f, -135.0f);
	this->addChild(m_plQuality);

	m_plAll = VerticalList::create(loaderManager->m_dAll);
	m_plAll->setPosition(-95.0f, 100.0f);
	m_plApplied = VerticalList::create(loaderManager->m_dApplied);
	m_plApplied->setPosition(95.0f, 100.0f);
	m_plAll->setTarget(m_plApplied);
	m_plApplied->setTarget(m_plAll);
	this->addChild(m_plAll);
	this->addChild(m_plApplied);

	this->setKeypadEnabled(true);
	this->setMouseEnabled(true);
	//this->scheduleUpdate();

	return true;
}

void LoaderLayer::update(float dt) {
	auto gl = CCEGLView::sharedOpenGLView();
	auto list = gl->getMousePosition().x > gl->getFrameSize().width / 2
		? m_plApplied : m_plAll;
	list->update(dt);
}

void LoaderLayer::keyBackClicked() {
	this->onExit(nullptr);
}

void LoaderLayer::scrollWheel(float x, float y) {
	auto gl = CCEGLView::sharedOpenGLView();
	auto list = gl->getMousePosition().x > gl->getFrameSize().width / 2 
		? m_plApplied : m_plAll;
	list->scroll(x);
}

void LoaderLayer::FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) {
	if (!btn2) return;
	if (layer->getTag() == kPlistWarningLayer) {
		auto loaderManager = LoaderManager::sharedState();
		if (loaderManager->m_bCheckQuality) {
			if (!loaderManager->checkQuality()) {
				auto alert = gd::FLAlertLayer::create(
					this, "Warning", "Cancel", "Yes", 300.0f, "Packs may not line up with <cy>quality</c>.\nDo you want to <cg>Continue</c>?"
				);
				alert->setTag(kQualityWarningLayer);
				alert->show();
				return;
			}
		}
	}
	this->reloadAll();
}

void LoaderLayer::onApply(cocos2d::CCObject*) {
	auto loaderManager = LoaderManager::sharedState();
	if (loaderManager->m_bCheckPlists) {
		if (!loaderManager->checkPlists()) {
			auto alert = gd::FLAlertLayer::create(
				this, "Warning", "Cancel", "Yes", 300.0f, "Packs may display <cr>incorrectly</c>.\nDo you want to <cg>Continue</c>?"
			);
			alert->setTag(kPlistWarningLayer);
			alert->show();
			return;
		}
	}
	if (loaderManager->m_bCheckQuality) {
		if (!loaderManager->checkQuality()) {
			auto alert = gd::FLAlertLayer::create(
				this, "Warning", "Cancel", "Yes", 300.0f, "Packs may not line up with <cy>quality</c>.\nDo you want to <cg>Continue</c>?"
			);
			alert->setTag(kQualityWarningLayer);
			alert->show();
			return;
		}
	}
	this->reloadAll();
}

void LoaderLayer::onDropDown(cocos2d::CCObject*) {
	auto dropDown = LoaderDropDownLayer::create();
	this->addChild(dropDown);
	dropDown->setPosition(0.0f, 0.0f);
	dropDown->showLayer(false);
}

void LoaderLayer::onExit(cocos2d::CCObject*) {
	auto scene = CCScene::create();
	scene->addChild(gd::MenuLayer::create());

	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
}

void LoaderLayer::onRefresh(cocos2d::CCObject*) {
	std::string text = LoaderManager::sharedState()->updatePacks(true);
	m_plAll->updateList();
	m_plApplied->updateList();

	auto alert = gd::FLAlertLayer::create(nullptr, "Update", "OK", nullptr, 350.0f, text);
	alert->show();
}

void LoaderLayer::onFolder(cocos2d::CCObject*) {
	CCApplication::sharedApplication()->openURL("packs");
}

void LoaderLayer::reloadAll() {
	auto loaderManager = LoaderManager::sharedState();

	CCDirector::sharedDirector()->updateContentScale(
		static_cast<TextureQuality>(loaderManager->m_dQuality.m_uOffset + 1)
	);

	auto gameManager = gd::GameManager::sharedState();
	gameManager->setQuality(
		static_cast<TextureQuality>(loaderManager->m_dQuality.m_uOffset + 1)
	);
	gameManager->reloadAll(false, false, true);
}

LoaderLayer* LoaderLayer::create() {
	LoaderLayer* pRet = new LoaderLayer();
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

void __fastcall LoaderLayer::scene(gd::MenuLayer* This, void*, cocos2d::CCObject* btn) {
	auto scene = CCScene::create();
	scene->addChild(LoaderLayer::create());

	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
}