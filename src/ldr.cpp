#include "pch.h"
#include "ldr.h"

namespace ldr {
    BTN_CALLBACK(exitScene);
    BTN_CALLBACK(left);
    BTN_CALLBACK(right);
    BTN_CALLBACK(apply);

    namespace vars {
        enum textureQuality {
            LOW = 1,
            MEDIUM,
            HIGH
        };

        std::vector<std::string> packList{};
        std::string currentPack{ "Resources" };
        int index;

        void* packLabel;
        void* enterBtn;
        bool bTransition{ true };

        cocos::fileUtils::tAddPath gAddPath;
    }

    namespace hooks {
        COCOS_HOOK addPath(void* CCFileUtils, void* __EDX, const char* path) {
            using namespace vars;

            if (currentPack == "Resources") {
                return gAddPath(CCFileUtils, path);
            }
            else {
                gAddPath(CCFileUtils, currentPack.c_str());
                return gAddPath(CCFileUtils, path);
            }
        }

        COCOS_HOOK loadingFinished(void* LoadingLayer, void* __EDX) {
            using namespace vars;

            bTransition = false;
            exitScene(LoadingLayer);
        }
    }

    void getPacks() {
        using namespace std::filesystem;
        using namespace vars;

        packList.clear();
        path packs = current_path() / "packs";
        packList.push_back("Resources");
        if (exists(packs) && is_directory(packs)) {
            directory_iterator packsIter{ packs };
            for (directory_entry pack : packsIter) {
                if (is_directory(pack))
                    packList.push_back(pack.path().string().substr(pack.path().string().find("packs")));
            }
        }
        else if (exists(packs) && !is_directory(packs))
            MessageBox(0, "ERROR: packs is an existing file.\n please remove it to use textureldr.", "textureldr", MB_OK | MB_ICONERROR);
        else {
            //TODO: change this to fading text so it doesn't exit you out of fullscreen
            create_directories(packs);
            MessageBox(0, "created packs folder.", "textureldr", MB_OK);
        }
    }

	BTN_CALLBACK(enterScene) {
        using namespace cocos;
        using namespace gd;
        using namespace vars;

        //setup
        getPacks();

        //create scene here
        void* director = director::get();
        vec2 winSize = director::getWinSize(director);

        void* myScene = scene::create();

        void* myMenu = menu::create();
        node::addChild(myScene, myMenu);

        packLabel = label::create(packList[index].c_str(), "bigFont.fnt");
        node::setPos(packLabel, winSize.x / 2, winSize.y / 2 + 60.0f);
        node::addChild(myScene, packLabel);

        void* applyBtn = menuItem::createImg("GJ_button_01.png", "GJ_button_02.png", 0, myMenu, apply);
        node::setPos(applyBtn, 0.0f, 0.0f);
        node::addChild(myMenu, applyBtn);

        void* backSprite = sprite::create("GJ_arrow_01_001.png");
        void* backBtn = menuItem::createSpr(backSprite, backSprite, 0, myMenu, exitScene);
        node::setPos(backBtn, -winSize.x / 2 + 25.0f, winSize.y / 2 - 25.0f);
        node::addChild(myMenu, backBtn);

        void* navSprite = sprite::create("navArrowBtn_001.png");
        void* leftBtn = menuItem::createSpr(navSprite, navSprite, 0, myMenu, left);
        node::setPos(leftBtn, -winSize.x / 2 + 25.0f, 0.0f);
        node::setRot(leftBtn, 180.0f);
        node::addChild(myMenu, leftBtn);
        void* rightBtn = menuItem::createSpr(navSprite, navSprite, 0, myMenu, right);
        node::setPos(rightBtn, winSize.x / 2 - 25.0f, 0.0f);
        node::addChild(myMenu, rightBtn);

        void* transition = transition::create(0.5f, myScene);
        director::replaceScene(director, transition);
	}

	BTN_CALLBACK(exitScene) {
        using namespace cocos;
        using namespace gd;
        using namespace vars;

        void* director = director::get();
        vec2 winSize = director::getWinSize(director);

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
    
    BTN_CALLBACK(left) {
        using namespace cocos;
        using namespace vars;

        if (index > 0) {
            --index;
            label::set(packLabel, packList[index].c_str(), true);
        }
    }

    BTN_CALLBACK(right) {
        using namespace cocos;
        using namespace vars;

        if (index < packList.size() - 1) {
            ++index;
            label::set(packLabel, packList[index].c_str(), true);
        }
    }

    BTN_CALLBACK(apply) {
        using namespace cocos;
        using namespace gd;
        using namespace vars;

        void* director = director::get();
        director::updateScale(director, HIGH);
        *(gd::gamemanager::ptr + 0xB8) = HIGH;
        currentPack = packList[index];
        gamemanager::reloadAll(gd::gamemanager::ptr, 0, 0, 1);
    }

    bool init() {
        using namespace vars;

        index = 0;
        return true;
    }
}