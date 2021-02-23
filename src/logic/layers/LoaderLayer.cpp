#include "pch.h"
#include "LoaderLayer.h"

using namespace cocos2d;

HorizontalList LoaderLayer::s_lQuality = { "Quality" };

VerticalList LoaderLayer::s_lAll = { "Available", 10, &s_lApplied };
VerticalList LoaderLayer::s_lApplied = { "Applied", 10, &s_lAll };

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
	this->addChild(bottomLeft);
	this->addChild(bottomRight);

	auto buttons = CCMenu::create();
	this->addChild(buttons);

	auto applyBtn = gd::CCMenuItemSpriteExtra::create(
		gd::ButtonSprite::create("Apply", 0, false, "goldFont.fnt", "GJ_button_01.png", 0.0f, 1.0f),
		this,
		menu_selector(LoaderLayer::onApply)
	);
	applyBtn->setPosition(0.0f, 0.0f);
	buttons->addChild(applyBtn);

	auto backBtn = gd::CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
		this,
		menu_selector(LoaderLayer::onExit)
	);
	backBtn->setPosition(-winSize.width / 2 + 25.0f, winSize.height / 2 - 25.0f);
	buttons->addChild(backBtn);

	auto reloadBtn = gd::CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png"),
		this,
		menu_selector(LoaderLayer::getPacks)
	);
	reloadBtn->setPosition(winSize.width / 2 - 35.0f, -winSize.height / 2 + 35.0f);
	buttons->addChild(reloadBtn);

	auto optionsBtn = gd::CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png"),
		this,
		menu_selector(LoaderLayer::onOptions)
	);
	optionsBtn->setPosition(-winSize.width / 2 + 35.0f, -winSize.height / 2 + 35.0f);
	buttons->addChild(optionsBtn);

	s_lQuality.enter(this);
	s_lAll.enter(this);
	s_lApplied.enter(this);

	this->setKeypadEnabled(true);

	return true;
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

void LoaderLayer::keyBackClicked() {
	this->onExit(nullptr);
}

void LoaderLayer::onApply(cocos2d::CCObject*) {
	s_lQuality.exit();
	s_lAll.exit();
	s_lApplied.exit();

	CCDirector::sharedDirector()->updateContentScale(static_cast<TextureQuality>(s_lQuality.getIndex() + 1));

	auto gameManager = gd::GameManager::sharedState();
	gameManager->setQuality(static_cast<TextureQuality>(s_lQuality.getIndex() + 1));
	gameManager->reloadAll(false, false, true);
}

void LoaderLayer::onOptions(cocos2d::CCObject*) {
	//EDIT
}

void LoaderLayer::onExit(cocos2d::CCObject*) {
	s_lQuality.exit();
	s_lAll.exit();
	s_lApplied.exit();

	auto scene = CCScene::create();
	scene->addChild(gd::MenuLayer::create());

	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
}

void LoaderLayer::getPacks(cocos2d::CCObject*) {
	std::string text = LoaderLayer::getPacks(true);
	auto alert = gd::FLAlertLayer::create(nullptr, "Update", "OK", nullptr, text);
	alert->show();
}

std::string LoaderLayer::getPacks(bool generate) {
	using namespace std::filesystem;

	std::vector<std::string> packsList = {};

	/*all characters allowed by stock bigFont.fnt and goldFont.fnt.
	* excludes '\', '/', ':', '*', '?', '"', '<', '>', and '|' since windows
	* folders can't have those symbols, and '•' since it acts weird and i can't
	* be bothered lol
	*/
	constexpr std::string_view filter = " !#$%&'()+,-.0123456789;=@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{}~";
	unsigned int invalid = 0;

	Log::info("Searching for packs...");
	path packs = current_path() / "packs";
	if (exists(packs)) {
		if (is_directory(packs)) {
			Log::info("Packs directory found. Iterating...");
			for (directory_entry pack : directory_iterator{ packs }) {
				if (is_directory(pack)) {
					bool valid = true;
					for (auto c : pack.path().filename().u8string()) {
						if (filter.find(c) == filter.npos) {
							valid = false;
							break;
						}
					}

					if (valid)
						packsList.push_back(pack.path().filename().string());
					else
						++invalid;
				}
			}

			Log::info(packsList.size(), " packs found.");
		}
		else
			MessageBox(0, "ERROR: packs is an existing file.\n please remove it to use textureldr.", "textureldr", MB_OK | MB_ICONERROR);
	}
	else {
		Log::error("No packs directory found. Creating new directory...");
		create_directories(packs);
	}

	unsigned int added = 0;
	unsigned int removed = 0;

	if (!(s_lAll.getEntries().empty() && s_lApplied.getEntries().empty())) {
		added = s_lAll.ifNotFound(packsList, true);
		removed = s_lAll.ifNotFound(packsList, false);
	}
	else {
		s_lAll.setEntries(packsList);
		added = packsList.size();
	}

	if (generate) {
		std::stringstream text = {};
		if (added != 0 || removed != 0) {
			text << "<cg>" << added << "</c> " << ((added == 1) ? "pack" : "packs") << " added.\n";
			//for some silly reason, color markers don't work directly after a newline!
			//@robtop fix???
			text << " <cr>" << removed << "</c> " << ((removed == 1) ? "pack" : "packs") << " removed.\n";
		}
		else {
			text << "<cy>Nothing changed!</c>\n";
		}

		if (invalid > 0) {
			text << '\n';
			if (invalid == 1) {
				text << "1 pack had an <co>invalid</c> name, and was ignored.\n";
			}
			else {
				text << invalid << " packs had <co>invalid</c> names, and were ignored.\n";
			}
		}
		return text.str();
	}
	else {
		return "";
	}
}

bool LoaderLayer::load() {
	using namespace tinyxml2;

	Log::info("Attempting to load savefile...");
	s_pFile = new XMLDocument();
	if (s_pFile->LoadFile(s_sFilePath)) {
		Log::error("Could not load config.dat. Trying backup.dat...");
		if (s_pFile->LoadFile(s_sBackupPath)) {
			Log::error("Could not load backup.dat.");
			return false;
		}

		Log::info("Loaded backup.dat. Saving...");
		s_pFile->SaveFile(s_sFilePath, false);
	}
	else {
		Log::info("Loaded config.dat. Saving...");
		s_pFile->SaveFile(s_sBackupPath, false);
	}

	if (!s_lQuality.load(s_pFile) ||
		!s_lAll.load(s_pFile) ||
		!s_lApplied.load(s_pFile)) {
		return false;
	}
	return true;
}

bool LoaderLayer::save() {
	using namespace tinyxml2;

	s_pFile->DeleteChildren();

	if (!s_lQuality.save(s_pFile) ||
		!s_lAll.save(s_pFile) ||
		!s_lApplied.save(s_pFile)) {
		return false;
	}

	return s_pFile->SaveFile(s_sFilePath) == XML_SUCCESS;
}

const std::vector<std::string>& LoaderLayer::getApplied() {
	return s_lApplied.getEntries();
}

bool init() {
	if (!LoaderLayer::load()) {
		LoaderLayer::getPacks(false);
		LoaderLayer::s_lQuality.setEntries({ "Low", "Medium", "High" });
		LoaderLayer::s_lQuality.setOffset(2);
	}
	LoaderLayer::s_lQuality.setPosition(0.0f, -130.0f);
	LoaderLayer::s_lAll.setPosition(-120.0f, 95.0f);
	LoaderLayer::s_lApplied.setPosition(120.0f, 95.0f);

	return true;
}

void __stdcall enter(cocos2d::CCObject*) {
	auto scene = CCScene::create();
	scene->addChild(LoaderLayer::create());

	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
}