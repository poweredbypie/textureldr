#include "pch.h"
#include "memory/Tools.h"
#include "memory/Hooks.h"
#include "logic/layers/LoaderLayer.h"

DWORD WINAPI Entry(LPVOID hModule) {
    if (!Log::init()) {
        MessageBox(0, "ERROR: Could not initialize logging.", "textureldr", MB_ICONERROR | MB_OK);
    }
    if (gd::init() && init()) {
        Log::info("Setting up hooks.");

        Hook LoadingLayer_loadingFinished = {
        gd::base + 0x18C790,
        hooks::LoadingLayer_loadingFinished,
        &gates::LoadingLayer_loadingFinished,
        7
        };

        Hook GameManager_dataLoaded = {
            gd::base + 0xCC500,
            hooks::GameManager_dataLoaded,
            &gates::GameManager_dataLoaded,
            6
        };

        Hook AppDelegate_trySaveGame = {
            gd::base + 0x3D5E0,
            hooks::AppDelegate_trySaveGame,
            &gates::AppDelegate_trySaveGame,
            11
        };

        HMODULE cocosBase = GetModuleHandleA("libcocos2d.dll");
        if (!cocosBase) {
            Log::error("Could not find cocos library.");
            goto exit;
        }

        Hook CCFileUtils_addSearchPath = {
            GetProcAddress(cocosBase, "?addSearchPath@CCFileUtils@cocos2d@@UAEXPBD@Z"),
            hooks::CCFileUtils_addSearchPath,
            &gates::CCFileUtils_addSearchPath,
            5
        };

        LoadingLayer_loadingFinished.enable();
        GameManager_dataLoaded.enable();
        AppDelegate_trySaveGame.enable();
        CCFileUtils_addSearchPath.enable();

        Log::info("Patching bytes.");
        patch(
            addrs::ppMoreGamesBtn,
            &addrs::pFolderBtn,
            nullptr,
            4
        );
        patch(
            addrs::pSpriteSize,
            "\x00\x00\xC0\x3F",
            nullptr,
            4
        );
        patch(
            addrs::ppCallback,
            &addrs::pNewCallback,
            nullptr,
            4
        );
    }
    else {
    exit:
        Log::error("Could not find module base addresses. Exiting.");
        FreeLibraryAndExitThread(static_cast<HMODULE>(hModule), 0);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        HANDLE _ = CreateThread(0, 0, Entry, hModule, 0, nullptr);
        if (_)
            CloseHandle(_);
    }
    return TRUE;
}