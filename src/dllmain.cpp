// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "extern/cocos.h"
#include "extern/gd.h"
#include "classes/hk.h"
#include "classes/list.h"
#include "ldr.h"

BOOL WINAPI attach(HMODULE hModule) {
    //find cocos & gd function offsets
    if (cocos::init() && gd::init() && ldr::init()) {
        //hook loadingFinished, addPath, and trySaveGame
        hk loadingFinished{ (char*)gd::loadingLayer::loadingFinished, (char*)ldr::hooks::loadingFinished, 0, 7 };
        hk addPath{ (char*)cocos::fileUtils::addPath, (char*)ldr::hooks::addPath, (char**)&ldr::gates::addPath, 5 };
        hk trySaveGame{ (char*)gd::appDelegate::trySaveGame, (char*)ldr::hooks::trySaveGame, (char**)&ldr::gates::trySaveGame, 11 };
        loadingFinished.hook();
        addPath.hook();
        trySaveGame.hook();

        void* cbEnterBtnAddr = &ldr::enterScene;

        //patch params for the "more games" button to change it into the textureldr menu button, since it's basically useless
        //save the bytes so they can be restored
        patch((char*)gd::menuLayer::pMoreGamesStr, (char*)&gd::menuLayer::oFolderBtnStr, 0, 4);
        patch((char*)gd::menuLayer::szMoreGamesBtn, (char*)"\x00\x00\xC0\x3F", 0, 4);
        patch((char*)gd::menuLayer::pcbMoreGames, (char*)&cbEnterBtnAddr, (char*)&gd::menuLayer::cbMoreGames, 4);

        //keep thread alive
        MessageBox(0, "textureldr active.\n press OK to unhook and exit.", "textureldr", MB_OK);

        //unhook and unpatch, save the config files
        loadingFinished.unhook();
        addPath.unhook();
        trySaveGame.unhook();
        listManager::save();

        patch((char*)gd::menuLayer::pMoreGamesStr, (char*)&gd::menuLayer::oMoreGamesStr, 0, 4);
        patch((char*)gd::menuLayer::szMoreGamesBtn, (char*)"\x66\x66\x66\x3F", 0, 4);
        patch((char*)gd::menuLayer::pcbMoreGames, (char*)&gd::menuLayer::cbMoreGames, 0, 4);
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
        
        if (dllHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)attach, hModule, 0, 0))
            CloseHandle(dllHandle);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

