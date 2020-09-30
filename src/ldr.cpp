#include "pch.h"
#include "ldr.h"

namespace ldr {
    BTN_CALLBACK(exitScene);
    BTN_CALLBACK(apply);

    namespace vars {
        list quality{ "quality", 1 };

        extern listExt applied;
        listExt all{ "available", 10, &applied};
        listExt applied{ "applied", 10, &all};

        bool bTransition{ true };

        cocos::fileUtils::tAddPath gAddPath;
    }

    namespace hooks {
        void COCOS_HOOK addPath(void* CCFileUtils, void* __EDX, const char* path) {
            using namespace vars;

            for (int i{}; i < (int)applied.getArray().size(); ++i) {
                gAddPath(CCFileUtils, ("packs\\" + applied.getArray()[i]).c_str());
            }
            return gAddPath(CCFileUtils, path);
        }

        void COCOS_HOOK loadingFinished(void* LoadingLayer) {
            using namespace vars;

            bTransition = false;
            exitScene(LoadingLayer);
        }
    }

    std::vector<std::string> getPacks() {
        using namespace std::filesystem;

        std::vector<std::string> packList{};

        path packs = current_path() / "packs";
        if (exists(packs)) {
            if (is_directory(packs)) {
                directory_iterator packsIter{ packs };
                for (directory_entry pack : packsIter) {
                    if (is_directory(pack))
                        packList.push_back(pack.path().filename().string());
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
        return packList;
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

        void* navSprite = sprite::create("navArrowBtn_001.png");
        /*void* leftBtn = menuItem::createSpr(navSprite, navSprite, 0, miscBtns, left);
        node::setPos(leftBtn, -winSize.x / 2 + 25.0f, 0.0f);
        node::setRot(leftBtn, 180.0f);
        node::addChild(miscBtns, leftBtn);
        void* rightBtn = menuItem::createSpr(navSprite, navSprite, 0, miscBtns, right);
        node::setPos(rightBtn, winSize.x / 2 - 25.0f, 0.0f);
        node::addChild(miscBtns, rightBtn);*/

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
        *(gd::gamemanager::ptr + 0xB8) = quality.getCurrentIndex() + 1;
        gamemanager::reloadAll(gd::gamemanager::ptr, 0, 0, 1);
    }

    bool init() {
        using namespace vars;

        quality.setArray({"LOW", "MEDIUM", "HIGH"});
        quality.setPosition(0.0f, -130.0f);

        all.setArray(getPacks());
        all.setPosition(-90.0f, 95.0f);
        applied.setPosition(90.0f, 95.0f);

        return true;
    }
}