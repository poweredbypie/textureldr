#include "pch.h"
#include "ldr.h"

namespace ldr {
    BTN_CALLBACK(exitScene);
    BTN_CALLBACK(left);
    BTN_CALLBACK(right);
    BTN_CALLBACK(apply);

    enum textureQuality {
        LOW = 1,
        MEDIUM,
        HIGH
    };

    namespace vars {
        listManager manager;

        extern list applied;
        list all{ "available", &applied, &manager };
        list applied{ "applied", &all, &manager };

        bool bTransition{ true };

        cocos::fileUtils::tAddPath gAddPath;
    }

    namespace hooks {
        void COCOS_HOOK addPath(void* CCFileUtils, void* __EDX, const char* path) {
            using namespace vars;

            for (int i{}; i < applied.getArray().size(); ++i) {
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

	BTN_CALLBACK(enterScene) {
        using namespace cocos;
        using namespace gd;
        using namespace vars;

        //create scene here
        void* director = director::get();
        vec2 winSize = director::getWinSize(director);

        void* ldrScene = scene::create();

        manager.enter(ldrScene);

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

        manager.exit();

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

    BTN_CALLBACK(left) {

    }

    BTN_CALLBACK(right) {

    }

    BTN_CALLBACK(apply) {
        using namespace cocos;
        using namespace gd;
        using namespace vars;

        void* director = director::get();
        director::updateScale(director, HIGH);
        *(gd::gamemanager::ptr + 0xB8) = HIGH;
        gamemanager::reloadAll(gd::gamemanager::ptr, 0, 0, 1);
    }

    bool init() {
        using namespace vars;

        all.loadArray();
        all.setPosition(-90.0f, 90.0f);
        applied.setPosition(90.0f, 90.0f);

        return true;
    }
}