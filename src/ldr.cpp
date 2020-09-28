#include "pch.h"
#include "ldr.h"

namespace ldr {
    BTN_CALLBACK(exitScene);
    BTN_CALLBACK(up);
    BTN_CALLBACK(down);
    BTN_CALLBACK(addToApplied);
    BTN_CALLBACK(appliedUp);
    BTN_CALLBACK(appliedDown);
    BTN_CALLBACK(swapUp);
    BTN_CALLBACK(swapDown);
    BTN_CALLBACK(removeFromApplied);
    BTN_CALLBACK(apply);

    enum textureQuality {
        LOW = 1,
        MEDIUM,
        HIGH
    };

    namespace vars {
        list all;
        list applied;

        bool bTransition{ true };

        cocos::fileUtils::tAddPath gAddPath;
    }

    namespace hooks {
        void COCOS_HOOK addPath(void* CCFileUtils, void* __EDX, const char* path) {
            using namespace vars;

            for (int i{}; i < applied.strings.size(); ++i) {
                gAddPath(CCFileUtils, ("packs\\" + applied.strings[i]).c_str());
            }
            return gAddPath(CCFileUtils, path);
        }

        void COCOS_HOOK loadingFinished(void* LoadingLayer) {
            using namespace vars;

            bTransition = false;
            exitScene(LoadingLayer);
        }
    }

    void getPacks() {
        using namespace std::filesystem;
        using namespace vars;

        all.strings.clear();
        path packs = current_path() / "packs";
        if (exists(packs)) {
            if (is_directory(packs)) {
                directory_iterator packsIter{ packs };
                for (directory_entry pack : packsIter) {
                    if (is_directory(pack))
                        all.strings.push_back(pack.path().filename().string());
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
    }

	BTN_CALLBACK(enterScene) {
        using namespace cocos;
        using namespace gd;
        using namespace vars;

        //create scene here
        void* director = director::get();
        vec2 winSize = director::getWinSize(director);

        void* ldrScene = scene::create();

        all.enter(up, down, addToApplied, ldrScene);
        applied.enter(appliedUp, appliedDown, removeFromApplied, swapUp, swapDown, ldrScene);

        void* miscBtns = menu::create();
        node::addChild(ldrScene, miscBtns);

        void* applyBtn = menuItem::createImg("GJ_button_01.png", "GJ_button_02.png", 0, miscBtns, apply);
        node::setPos(applyBtn, 0.0f, 0.0f);
        node::addChild(miscBtns, applyBtn);

        void* backSprite = sprite::create("GJ_arrow_01_001.png");
        void* backBtn = menuItem::createSpr(backSprite, backSprite, 0, miscBtns, exitScene);
        node::setPos(backBtn, -winSize.x / 2 + 25.0f, winSize.y / 2 - 25.0f);
        node::addChild(miscBtns, backBtn);

        if (all.strings.size() == 0) {
            menuItem::setEnabled(all.sel, false);
            node::setVisible(all.sel, false);
        }
        /*void* leftBtn = menuItem::createSpr(navSprite, navSprite, 0, myMenu, left);
        node::setPos(leftBtn, -winSize.x / 2 + 25.0f, 0.0f);
        node::setRot(leftBtn, 180.0f);
        node::addChild(myMenu, leftBtn);
        void* rightBtn = menuItem::createSpr(navSprite, navSprite, 0, myMenu, right);
        node::setPos(rightBtn, winSize.x / 2 - 25.0f, 0.0f);
        node::addChild(myMenu, rightBtn);*/

        void* transition = transition::create(0.5f, ldrScene);
        director::replaceScene(director, transition);
	}

	BTN_CALLBACK(exitScene) {
        using namespace cocos;
        using namespace gd;
        using namespace vars;

        all.exit();
        applied.exit();

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
    
    BTN_CALLBACK(up) {
        using namespace vars;

        if (all.selIndex > 0)
            --all.selIndex;
        else if (all.index > 0)
            --all.index; 
        all.update();
    }

    BTN_CALLBACK(down) {
        using namespace vars;

        if (all.selIndex < all.length - 1)
            ++all.selIndex;
        else if (all.index < (int)all.strings.size() - LIST_LENGTH)
            ++all.index;
        all.update();
    }

    BTN_CALLBACK(addToApplied) {
        using namespace vars;

        applied.strings.insert(applied.strings.begin(), all.strings[all.selIndex + all.index]);
        all.strings.erase(all.strings.begin() + all.selIndex + all.index);

        //moves the selector up if the index is the bottom one, so that "vector out of range" doesn't occur
        if (all.strings.size() - all.index == all.selIndex && all.strings.size() - all.index != 0)
            --all.selIndex;
        all.update();
        applied.update();
    }
    
    BTN_CALLBACK(appliedUp) {
        using namespace vars;

        if (applied.selIndex > 0)
            --applied.selIndex;
        else if (applied.index > 0)
            --applied.index;
        applied.update();
    }

    BTN_CALLBACK(appliedDown) {
        using namespace vars;

        if (applied.selIndex < applied.length - 1)
            ++applied.selIndex;
        else if (applied.index < (int)applied.strings.size() - LIST_LENGTH)
            ++applied.index;
        applied.update();
    }

    BTN_CALLBACK(removeFromApplied) {
        using namespace vars;

        all.strings.insert(all.strings.begin(), applied.strings[applied.selIndex + applied.index]);
        applied.strings.erase(applied.strings.begin() + applied.selIndex + applied.index);

        //moves the selector up if the index is the bottom one, so that "vector out of range" doesn't occur
        if (applied.strings.size() - applied.index == applied.selIndex && applied.strings.size() - applied.index != 0)
            --applied.selIndex;
        all.update();
        applied.update();
    }

    BTN_CALLBACK(swapUp) {
        using namespace vars;

        if (applied.selIndex + applied.index != 0) {
            std::iter_swap(applied.strings.begin() + applied.selIndex + applied.index, applied.strings.begin() + applied.selIndex + applied.index - 1);
            --applied.selIndex;
            applied.update();
        }
    }

    BTN_CALLBACK(swapDown) {
        using namespace vars;

        if (applied.selIndex + applied.index + 1 != applied.strings.size()) {
            std::iter_swap(applied.strings.begin() + applied.selIndex + applied.index, applied.strings.begin() + applied.selIndex + applied.index + 1);
            ++applied.selIndex;
            applied.update();
        }
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

        getPacks();

        all.x = -90.0f;
        all.y = 90.0f;
        applied.x = 90.0f;
        applied.y = 90.0f;
        return true;
    }
}