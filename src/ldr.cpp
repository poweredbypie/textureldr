#include "pch.h"
#include "ldr.h"

namespace ldr {
    BTN_CALLBACK(exitScene);
    BTN_CALLBACK(apply);

    //so that i can add this DLL to an import table.
    void __declspec(dllexport) 派() {};

    namespace vars {
        list quality = { "Quality", 1 };

        extern listExt applied;
        listExt all = { "Available", 10, &applied };
        listExt applied = { "Applied", 10, &all };
    }

    namespace hooks {
        void COCOS_HOOK loadingFinished(gd::LoadingLayer* This) {
            This->setWillFadeIn(false);
            listManager::exit();
            return gates::loadingFinished(This);
        }

        void COCOS_HOOK dataLoaded(gd::GameManager* This, void*, void* DS_Dictionary) {
            using namespace cocos2d;
            using namespace vars;

            auto fileUtils = CCFileUtils::sharedFileUtils();
            //remove all search paths that have been added.
            fileUtils->removeAllPaths();
            gates::addSearchPath(fileUtils, "Resources");
            //call dataLoaded.
            gates::dataLoaded(This, DS_Dictionary);
            //add all of 'em back lol
            fileUtils->removeAllPaths();
            for (std::string i : applied.getVector()) {
                gates::addSearchPath(fileUtils, ("packs/" + i).c_str());
            }
            gates::addSearchPath(fileUtils, "Resources");
        }

        void COCOS_HOOK trySaveGame(cocos2d::CCObject* AppDelegate) {
            listManager::save();
            return gates::trySaveGame(AppDelegate);
        }

        void COCOS_HOOK addSearchPath(cocos2d::CCFileUtils* This, void*, const char* path) {
            using namespace vars;

            //to refresh menuLoop.mp3.
            gd::MenuLayer::fadeInMusic(" ");
            for (std::string i : applied.getVector()) {
                gates::addSearchPath(This, ("packs/" + i).c_str());
            }
            return gates::addSearchPath(This, path);
        }
    }

    BTN_CALLBACK(getPacks) {
        using namespace std::filesystem;
        using namespace cocos2d;
        using namespace vars;

        std::vector<std::string> packsList = {};

        /*all characters allowed by stock bigFont.fnt and goldFont.fnt.
        * excludes '\', '/', ':', '*', '?', '"', '<', '>', and '|' since windows
        * folders can't have those symbols, and '•' since it acts weird and i can't
        * be bothered lol
        */
        constexpr std::string_view filter = " !#$%&'()+,-.0123456789;=@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{}~";
        int invalid = 0;

        log::info("Searching for packs...");
        path packs = current_path() / "packs";
        if (exists(packs)) {
            if (is_directory(packs)) {
                log::info("Packs directory found. Iterating...");
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

                log::info(packsList.size(), " packs found.");
            }
            else
                MessageBox(0, "ERROR: packs is an existing file.\n please remove it to use textureldr.", "textureldr", MB_OK | MB_ICONERROR);
        }
        else {
            log::error("No packs directory found. Creating new directory...");
            create_directories(packs);
        }
            
        int added = 0;
        int removed = 0;

        if (!(all.getVector().empty() && applied.getVector().empty())) {
            added = all.ifNotFound(packsList, true);
            removed = all.ifNotFound(packsList, false);
        }
        else {
            all.setVector(packsList);
            added = packsList.size();
        }

        if (pSender) {
            auto director = CCDirector::sharedDirector();
            auto winSize = director->getWinSize();

            std::stringstream text = {};
            if (added > 0 || removed > 0) {
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

            auto alert = gd::FLAlertLayer::create(nullptr, "Update", "OK", nullptr, text.str());
            alert->show();
        }
    }

    BTN_CALLBACK(onOptions) {
        options::enter(pSender);
    }

	BTN_CALLBACK(enterScene) {
        using namespace cocos2d;

        //create scene here
        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();
        auto ldrScene = CCScene::create();

        auto bg = CCSprite::create("GJ_gradientBG.png");
        auto bgSize = bg->getTextureSize();

        bg->setAnchorPoint({ 0.0f, 0.0f });
        bg->setScaleX((winSize.width + 10.0f) / bgSize.width);
        bg->setScaleY((winSize.height + 10.0f) / bgSize.height);
        bg->setPosition(-5.0f, -5.0f);
        bg->setColor({ 0, 102, 255 });
        ldrScene->addChild(bg);
        
        auto bottomLeft = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
        auto cornerSize = bottomLeft->getTextureSize();
        bottomLeft->setPosition(cornerSize.width / 2, cornerSize.height / 2);
        auto bottomRight = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
        bottomRight->setFlipX(true);
        bottomRight->setPosition(winSize.width - cornerSize.width / 2, cornerSize.height / 2);
        ldrScene->addChild(bottomLeft);
        ldrScene->addChild(bottomRight);

        listManager::enter(ldrScene);

        //buttons
        auto miscBtns = CCMenu::create();
        ldrScene->addChild(miscBtns);
        auto applyBtn = gd::CCMenuItemSpriteExtra::create(
            gd::ButtonSprite::create("Apply", 0, false, "goldFont.fnt", "GJ_button_01.png", 0.0f, 1.0f),
            miscBtns,
            apply
        );
        applyBtn->setPosition(0.0f, 0.0f);
        miscBtns->addChild(applyBtn);
        
        auto backBtn = gd::CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
            miscBtns,
            exitScene
        );
        backBtn->setPosition(-winSize.width / 2 + 25.0f, winSize.height / 2 - 25.0f);
        miscBtns->addChild(backBtn);

        auto reloadBtn = gd::CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png"),
            miscBtns,
            getPacks
        );
        reloadBtn->setPosition(winSize.width / 2 - 35.0f, -winSize.height / 2 + 35.0f);
        miscBtns->addChild(reloadBtn);

        auto optionsBtn = gd::CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png"),
            miscBtns,
            onOptions
        );
        optionsBtn->setPosition(-winSize.width / 2 + 35.0f, -winSize.height / 2 + 35.0f);
        miscBtns->addChild(optionsBtn);

        director->replaceScene(CCTransitionFade::create(0.5f, ldrScene));
	}

	BTN_CALLBACK(exitScene) {
        using namespace cocos2d;
        using namespace vars;

        listManager::exit();

        auto menuLayer = gd::MenuLayer::create();

        auto menuLayerScene = CCScene::create();
        menuLayerScene->addChild(menuLayer);

        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, menuLayerScene));
	}

    BTN_CALLBACK(apply) {
        using namespace cocos2d;
        using namespace vars;

        auto director = CCDirector::sharedDirector();
        director->updateContentScale(static_cast<TextureQuality>(quality.getCurrentIndex() + 1));
        
        auto GameManager = gd::GameManager::sharedState();
        GameManager->setQuality(static_cast<TextureQuality>(quality.getCurrentIndex() + 1));

        GameManager->reloadAll(false, false, true);
    }

    bool init() {
        using namespace vars;
        
        listManager::setSaveTargets("packs/config.dat", "packs/backup.dat");

        if (!listManager::load()) {
            getPacks(nullptr);
            quality.setVector({ "Low", "Medium", "High" });
            //set to high for convenience sake. TODO: maybe autodetect, or use gd's autodetect?
            quality.setOffset(2);
        }
        quality.setPosition(0.0f, -130.0f);
        all.setPosition(-120.0f, 95.0f);
        applied.setPosition(120.0f, 95.0f);

        return true;
    }
}