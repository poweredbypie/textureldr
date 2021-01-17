#include "pch.h"
#include "ldr.h"

namespace ldr {
    BTN_CALLBACK(exitScene);
    BTN_CALLBACK(apply);

    //so that i can add this DLL to an import table.
    void __declspec(dllexport) 派() {};

    namespace vars {
        list quality{ "Quality", 1 };

        extern listExt applied;
        listExt all{ "Available", 10, &applied };
        listExt applied{ "Applied", 10, &all };

        bool bTransition{ true };
    }

    namespace hooks {
        void COCOS_HOOK addSearchPath(cocos2d::CCFileUtils* _this, void* __EDX, const char* path) {
            using namespace vars;

            //to refresh menuLoop.mp3.
            gd::MenuLayer::fadeInMusic(" ");
            for (int i{}; i < (int)applied.getVector().size(); ++i) {
                gates::addSearchPath(_this, ("packs\\" + applied.getVector()[i]).c_str());
            }
            return gates::addSearchPath(_this, path);
        }

        void COCOS_HOOK loadingFinished(cocos2d::CCScene* LoadingLayer) {
            using namespace vars;

            bTransition = false;
            exitScene(LoadingLayer);
        }

        void COCOS_HOOK trySaveGame(cocos2d::CCNode* AppDelegate) {
            listManager::save();
            return gates::trySaveGame(AppDelegate);
        }
    }

    BTN_CALLBACK(getPacks) {
        using namespace std::filesystem;
        using namespace vars;

        std::vector<std::string> packsList{};

        path packs = current_path() / "packs";
        if (exists(packs)) {
            if (is_directory(packs)) {
                directory_iterator iter{ packs };
                for (directory_entry pack : iter) {
                    if (is_directory(pack))
                        packsList.push_back(pack.path().filename().string());
                }
            }
            else
                MessageBox(0, "ERROR: packs is an existing file.\n please remove it to use textureldr.", "textureldr", MB_OK | MB_ICONERROR);
        }
        else
            create_directories(packs);

        if (!(all.getVector().empty() && applied.getVector().empty())) {
            all.ifNotFound(packsList, true);
            all.ifNotFound(packsList, false);
        }
        else 
            all.setVector(packsList);
    }

	BTN_CALLBACK(enterScene) {
        using namespace gd;
        using namespace cocos2d;

        //create scene here
        auto director = CCDirector::sharedDirector();
        CCSize winSize = director->getWinSize();
        auto ldrScene = CCScene::create();
        listManager::enter(ldrScene);
        auto miscBtns = CCMenu::create();
        ldrScene->addChild(miscBtns);
        ButtonSprite* applyBtn = ButtonSprite::create(
            CCSprite::create("GJ_button_04.png"),
            miscBtns,
            apply
        );
        miscBtns->addChild(applyBtn);
        auto checkmark = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
        miscBtns->addChild(checkmark);
        
        auto backBtn = ButtonSprite::create(
            CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
            miscBtns,
            exitScene
        );

        backBtn->setPosition(-winSize.width / 2 + 25.0f, winSize.height / 2 - 25.0f);
        miscBtns->addChild(backBtn);

        auto reloadBtn = ButtonSprite::create(
            CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png"),
            miscBtns,
            getPacks
        );
        reloadBtn->setPosition(winSize.width / 2 - 35.0f, -winSize.height / 2 + 35.0f);
        reloadBtn->setScale(1.1f);
        miscBtns->addChild(reloadBtn);

        auto transition = CCTransitionFade::create(0.5f, ldrScene);
        director->replaceScene(transition);
	}

	BTN_CALLBACK(exitScene) {
        using namespace cocos2d;
        using namespace gd;
        using namespace vars;

        listManager::exit();

        auto director = CCDirector::sharedDirector();

        auto menuLayerScene = CCScene::create();

        auto menuLayer = MenuLayer::create();
        //FIX
        menuLayerScene->addChild((CCNode*)menuLayer);

        if (bTransition) {
            auto transition = CCTransitionFade::create(0.5f, menuLayerScene);
            director->replaceScene(transition);
        }
        else {
            director->replaceScene(menuLayerScene);
            bTransition = true;
        }
	}

    BTN_CALLBACK(apply) {
        using namespace cocos2d;
        using namespace vars;

        auto director = CCDirector::sharedDirector();
        director->updateContentScale(static_cast<TextureQuality>(quality.getCurrentIndex() + 1));
        
        auto GameManager = gd::GameManager::sharedState();
        *(reinterpret_cast<char*>(GameManager) + 0x2E4) = quality.getCurrentIndex() + 1;

        GameManager->reloadAll(false, false, true);
    }

    bool init() {
        using namespace vars;

        listManager::setSaveTargets("packs\\config.dat", "packs\\backup.dat");

        if (!listManager::load()) {
            getPacks(0);
            quality.setVector({ "Low", "Medium", "High" });
        }
        quality.setPosition(0.0f, -130.0f);
        all.setPosition(-120.0f, 95.0f);
        applied.setPosition(120.0f, 95.0f);

        return true;
    }
}