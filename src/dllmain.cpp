// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "classes/hk.h"
#include "classes/list.h"
#include "ldr.h"

#ifndef NDEBUG
    #define LOG(x) printf(x)
#else
    #define LOG(x)
#endif

DWORD WINAPI attach(LPVOID hModule) {
    //find cocos & gd function offsets
    if (cocos2d::init() && gd::init() && ldr::init()) {
        //hook loadingFinished, addSearchPath, and trySaveGame
        hk loadingFinished = { 
            gd::LoadingLayer::loadingFinished, 
            ldr::hooks::loadingFinished, 
            nullptr, 
            7
        };
        //you can't cast member functions to addresses because virtual functions so this'll do for now
        hk addSearchPath = { 
            GetProcAddress(cocos2d::base, "?addSearchPath@CCFileUtils@cocos2d@@UAEXPBD@Z"),
            ldr::hooks::addSearchPath,
            &ldr::gates::addSearchPath,
            5 
        };
        hk trySaveGame = { 
            gd::AppDelegate::trySaveGame,
            ldr::hooks::trySaveGame,
            &ldr::gates::trySaveGame,
            11
        };

        loadingFinished.hook();
        addSearchPath.hook();
        trySaveGame.hook();

        auto cbEnterBtnAddr = &ldr::enterScene;

        //patch params for the "more games" button to change it into the textureldr menu button, since it's basically useless
        //save the bytes so they can be restored
        patch(
            gd::MenuLayer::pMoreGamesStr,
            &gd::MenuLayer::oFolderBtnStr,
            0,
            4
        );
        patch(
            gd::MenuLayer::szMoreGamesBtn,
            "\x00\x00\xC0\x3F",
            0,
            4
        );
        patch(
            gd::MenuLayer::pcbMoreGames,
            &cbEnterBtnAddr,
            &gd::MenuLayer::cbMoreGames,
            4
        );

        #ifndef NDEBUG  //allows for the dll to be ejected and reinjected for testing; shouldn't be necessary in release mode
        //keep thread alive
        MessageBox(0, "DEBUG: textureldr active.\n press OK to unhook and exit.", "textureldr", MB_OK);

        //unhook and unpatch, save the config files
        loadingFinished.unhook();
        addSearchPath.unhook();
        trySaveGame.unhook();
        listManager::save();

        patch(
            gd::MenuLayer::pMoreGamesStr,
            &gd::MenuLayer::oMoreGamesStr,
            0,
            4
        );
        patch(
            gd::MenuLayer::szMoreGamesBtn,
            "\x66\x66\x66\x3F",
            0,
            4
        );
        patch(
            gd::MenuLayer::pcbMoreGames,
            &gd::MenuLayer::cbMoreGames,
            0,
            4
        );

        FreeLibraryAndExitThread(static_cast<HMODULE>(hModule), 0);
        #endif  
    }
    else {
        MessageBox(0, "ERROR: could not find all functions! \n press OK to exit.", "textureldr", MB_OK | MB_ICONERROR);
        //free library since it won't do anything anyways
        FreeLibraryAndExitThread(static_cast<HMODULE>(hModule), 0);
    }

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    HANDLE dllHandle = nullptr;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        
        if (dllHandle = CreateThread(0, 0, attach, hModule, 0, nullptr))
            CloseHandle(dllHandle);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

