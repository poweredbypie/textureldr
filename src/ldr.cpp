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
    namespace gates {
        void(__thiscall* addPath)(void* CCFileUtils, const char* path);
        void(__thiscall* trySaveGame)(void* AppDelegate);
    }

    namespace hooks {
        void COCOS_HOOK addPath(void* CCFileUtils, void* __EDX, const char* path) {
            using namespace vars;

            for (int i{}; i < (int)applied.getVector().size(); ++i) {
                gates::addPath(CCFileUtils, ("packs\\" + applied.getVector()[i]).c_str());
            }
            return gates::addPath(CCFileUtils, path);
        }

        void COCOS_HOOK loadingFinished(void* LoadingLayer) {
            using namespace vars;

            bTransition = false;
            exitScene(LoadingLayer);
        }

        void COCOS_HOOK trySaveGame(void* AppDelegate) {
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
                directory_iterator packsIter{ packs };
                for (directory_entry pack : packsIter) {
                    if (is_directory(pack))
                        packsList.push_back(pack.path().filename().string());
                }
            }
            else {
                MessageBox(0, "ERROR: packs is an existing file.\n please remove it to use textureldr.", "textureldr", MB_OK | MB_ICONERROR);
            }
        }
        else {
            //TODO: change this to fading text so it doesn't exit you out of fullscreen
            create_directories(packs);
            MessageBox(0, "created packs folder.", "textureldr", MB_OK);
        }

        if (!(all.getVector().empty() && applied.getVector().empty())) {
            std::vector<std::string> oldPacks{ all.getVector() };
            oldPacks.insert(oldPacks.end(), applied.getVector().begin(), applied.getVector().end());

            for (std::string pack : packsList) {
                if (std::find(oldPacks.begin(), oldPacks.end(), pack) == oldPacks.end()) {
                    all.getVector().insert(all.getVector().begin(), pack);
                }
            }
            all.removeIfNotFound(packsList, false);
            applied.removeIfNotFound(packsList, false);
        }
        else {
            all.setVector(packsList);
        }
    }

	BTN_CALLBACK(enterScene) {
        using namespace cocos;
        using namespace gd;
        using namespace vars;

        //create scene here
        void* director = director::get();
        vec2 winSize = director::getWinSize(director);

        void* ldrScene = scene::create();

        listManager::enter(ldrScene);

        void* miscBtns = menu::create();
        node::addChild(ldrScene, miscBtns);

        void* applyBtn = menuItem::createImg("GJ_button_01.png", "GJ_button_02.png", 0, miscBtns, apply);
        node::setPos(applyBtn, 0.0f, 0.0f);
        node::addChild(miscBtns, applyBtn);

        void* backSprite = sprite::create("GJ_arrow_01_001.png");
        void* backBtn = menuItem::createSpr(backSprite, backSprite, 0, miscBtns, exitScene);
        node::setPos(backBtn, -winSize.x / 2 + 25.0f, winSize.y / 2 - 25.0f);
        node::addChild(miscBtns, backBtn);

        void* reloadSprite = sprite::create("GJ_updateBtn_001.png");
        void* reloadBtn = menuItem::createSpr(reloadSprite, reloadSprite, 0, miscBtns, getPacks);
        node::setPos(reloadBtn, winSize.x / 2 - 35.0f, -winSize.y / 2 + 35.0f);
        node::setScale(reloadBtn, 1.1f);
        node::addChild(miscBtns, reloadBtn);

        void* transition = transition::create(0.5f, ldrScene);
        director::replaceScene(director, transition);
	}

	BTN_CALLBACK(exitScene) {
        using namespace cocos;
        using namespace gd;
        using namespace vars;

        listManager::exit();

        void* director = director::get();

        void* menuLayerScene = scene::create();

        void* menuLayer = menuLayer::create();
        node::addChild(menuLayerScene, menuLayer);

        if (bTransition) {
            void* transition = transition::create(0.5f, menuLayerScene);
            director::replaceScene(director, transition);
        }
        else {
            director::replaceScene(director, menuLayerScene);
            bTransition = true;
        }
	}

    BTN_CALLBACK(apply) {
        using namespace cocos;
        using namespace gd;
        using namespace vars;

        void* director = director::get();
        director::updateScale(director, quality.getCurrentIndex() + 1);
        *((char*)gd::gamemanager::get() + 0x2E0) = quality.getCurrentIndex() + 1;
        gamemanager::reloadAll(gd::gamemanager::get(), 0, 0, 1);
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