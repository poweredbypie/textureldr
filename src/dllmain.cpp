// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "cocos.h"
#include "gd.h"
#include "hk.h"

namespace globals {
    //needed for texture management
    std::vector<std::string> packsList{};
    std::string currentPack{ "Resources" };
    int index;
    enum textureQuality {
        LOW = 1,
        MEDIUM,
        HIGH
    };

    //needed externally for changing
    void* myLabel;
    void* enterBtn;
    bool transition{ true };

    //gateways to hooked functions
    cocos::scheduler::tUpdate gUpdate;
    cocos::fileUtils::tAddPath gAddPath;

    void __stdcall cbBackBtn(void* pSender);

    //since cbBackBtn and hkLoadingFinished references
    void __stdcall cbEnterBtn(void* pSender);

    //hooks and callbacks
    void __fastcall hkAddPath(void* CCFileUtils, void* garbage, char* resources) {
        if (currentPack == "Resources") {
            return gAddPath(CCFileUtils, resources);
        }
        else {
            gAddPath(CCFileUtils, currentPack.c_str());
            return gAddPath(CCFileUtils, resources);
        }
    }

    void __fastcall hkLoadingFinished(void* LoadingLayer, void* garbage) {
        transition = false;
        cbBackBtn(LoadingLayer);
    }

    void __stdcall cbBackBtn(void* pSender) {
        using namespace cocos;
        using namespace gd;

        void* director = director::get();
        vec2 winSize = director::getWinSize(director);

        void* menuLayerScene = scene::create();

        void* menuLayer = menuLayer::create();
        node::addChild(menuLayerScene, menuLayer);

        void* enterBtnMenu = menu::create();
        node::addChild(menuLayer, enterBtnMenu);

        enterBtn = menuItem::createImg("GJ_button_01.png", "GJ_button_02.png", 0, enterBtnMenu, (void*)cbEnterBtn);
        node::setPos(enterBtn, -winSize.x / 2 + 25.0f, 10.0f);
        node::addChild(enterBtnMenu, enterBtn);

        if (transition) {
            void* transition = transition::create(0.5f, menuLayerScene);
            director::replaceScene(director, transition);
        }
        else {
            director::replaceScene(director, menuLayerScene);
            transition = true;
        }
    }

    void __stdcall cbApplyBtn(void* pSender) {
        using namespace cocos;
        using namespace gd;
        void* director = director::get();
        director::updateScale(director, HIGH);
        *(gd::gamemanager::ptr + 0xB8) = HIGH;
        currentPack = packsList[index];
        gamemanager::reloadAll(gd::gamemanager::ptr, 0, 0, 1);
    }

    void __stdcall cbLeftBtn(void* pSender) {
        using namespace cocos;
        if (index > 0) {
            --index;
            label::set(myLabel, packsList[index].c_str(), 1);
        }
    }

    void __stdcall cbRightBtn(void* pSender) {
        using namespace cocos;
        if (index < packsList.size() - 1) {
            ++index;
            label::set(myLabel, packsList[index].c_str(), 1);
        }
    }

    void __stdcall cbEnterBtn(void* pSender) {
        using namespace cocos;
        using namespace gd;
        using namespace std::filesystem;

        //check current packs
        
        packsList.clear();
        path packs = current_path() / "packs";
        packsList.push_back("Resources");
        if (exists(packs) && is_directory(packs)) {
            directory_iterator packsIter{ packs };
            for (directory_entry pack : packsIter) {
                if (is_directory(pack))
                    packsList.push_back(pack.path().string().substr(pack.path().string().find("packs")));
            }
        }
        else if (exists(packs) && !is_directory(packs))
            MessageBox(0, "ERROR: packs is an existing file.\n please remove it to use textureldr.", "textureldr", MB_OK | MB_ICONERROR);
        else {
            create_directories(packs);
            MessageBox(0, "created packs folder.", "textureldr", MB_OK);
        }
        //--- create scene here
        void* director = director::get();
        vec2 winSize = director::getWinSize(director);

        void* myScene = scene::create();

        void* myMenu = menu::create();
        node::addChild(myScene, myMenu);

        myLabel = label::create(packsList.at(index).c_str(), "bigFont.fnt");
        node::setPos(myLabel, winSize.x / 2, winSize.y / 2 + 60.0f);
        node::addChild(myScene, myLabel);

        void* applyBtn = menuItem::createImg("GJ_button_01.png", "GJ_button_02.png", 0, myMenu, (void*)cbApplyBtn);
        node::setPos(applyBtn, 0.0f, 0.0f);
        node::addChild(myMenu, applyBtn);

        void* backSprite = sprite::create("GJ_arrow_01_001.png");
        void* backBtn = menuItem::createSpr(backSprite, backSprite, 0, myMenu, (void*)cbBackBtn);
        node::setPos(backBtn, -winSize.x / 2 + 25.0f, winSize.y / 2 - 25.0f);
        node::addChild(myMenu, backBtn);

        void* navSprite = sprite::create("navArrowBtn_001.png");
        void* leftBtn = menuItem::createSpr(navSprite, navSprite, 0, myMenu, (void*)cbLeftBtn);
        node::setPos(leftBtn, -winSize.x / 2 + 25.0f, 0.0f);
        node::setRot(leftBtn, 180.0f);
        node::addChild(myMenu, leftBtn);
        void* rightBtn = menuItem::createSpr(navSprite, navSprite, 0, myMenu, (void*)cbRightBtn);
        node::setPos(rightBtn, winSize.x / 2 - 25.0f, 0.0f);
        node::addChild(myMenu, rightBtn);

        void* transition = transition::create(0.5f, myScene);
        director::replaceScene(director, transition);
    }
}

BOOL WINAPI attach(HMODULE hModule) {
    //find cocos & gd function offsets
    if (cocos::init() && gd::init()) {

        globals::index = 0;

        //hook addsearchpath and loadingfinished
        hk loadingFinished{ (char*)gd::loadingLayer::loadingFinished, (char*)globals::hkLoadingFinished, 0, 7 };
        hk addPath{ (char*)cocos::fileUtils::addPath, (char*)globals::hkAddPath, (char**)&globals::gAddPath, 5 };
        loadingFinished.hook();
        addPath.hook();

        void* cbBackBtnAddr = &globals::cbBackBtn;

        //patch callbacks that init menuLayer (from creatorlayer, garagelayer, levelselectlayer, reloadall)
        //save the bytes so they can be restored
        patch((char*)gd::menuLayer::cbGarage, (char*)&cbBackBtnAddr, (char*)&gd::menuLayer::oBytesGarage, 4);
        patch((char*)gd::menuLayer::cbLevelLayer, (char*)&cbBackBtnAddr, (char*)&gd::menuLayer::oBytesLevelLayer, 4);
        patch((char*)gd::menuLayer::cbCreator, (char*)&cbBackBtnAddr, (char*)&gd::menuLayer::oBytesCreator, 4);

        //keep thread alive
        MessageBox(0, "textureldr active.\n press OK to unhook and exit.", "textureldr", MB_OK);

        loadingFinished.unhook();
        addPath.unhook();
        patch((char*)gd::menuLayer::cbGarage, (char*)&gd::menuLayer::oBytesGarage, 0, 4);
        patch((char*)gd::menuLayer::cbLevelLayer, (char*)&gd::menuLayer::oBytesLevelLayer, 0, 4);
        patch((char*)gd::menuLayer::cbCreator, (char*)&gd::menuLayer::oBytesCreator, 0, 4);

        //unhook and unpatch

    }
    else {
        MessageBox(0, "ERROR: could not find all functions!\n press OK to exit.", "textureldr", MB_OK | MB_ICONERROR);
    }

    //free library
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    HANDLE dllHandle{};
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        dllHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)attach, hModule, 0, 0);
        if (dllHandle)
            CloseHandle(dllHandle);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

