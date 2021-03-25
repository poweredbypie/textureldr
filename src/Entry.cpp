#include "pch.h"
#include "memory/Tools.h"
#include "memory/Hooks.h"
#include "logic/nodes/LoaderManager.h"
#include "logic/integration/HackProDelegate.h"

DWORD WINAPI Entry(LPVOID hModule) {
	try {
		if (!Log::init()) {
			MessageBox(nullptr, "ERROR: Could not initialize logging.", "textureldr", MB_OK | MB_ICONERROR);
		}
		if (gd::init() && LoaderManager::sharedState()) {
			Log::info("Setting up hooks.");

			Hook{
			gd::base + 0x18C790,
			hooks::LoadingLayer_loadingFinished,
			&gates::LoadingLayer_loadingFinished,
			7
			}.enable();

			Hook{
				gd::base + 0xCC500,
				hooks::GameManager_dataLoaded,
				&gates::GameManager_dataLoaded,
				6
			}.enable();

			Hook{
				gd::base + 0x3D5E0,
				hooks::AppDelegate_trySaveGame,
				&gates::AppDelegate_trySaveGame,
				11
			}.enable();

			HMODULE cocosBase = GetModuleHandleA("libcocos2d.dll");
			if (!cocosBase) goto exit;

			Hook{
				GetProcAddress(cocosBase, "?addSearchPath@CCFileUtils@cocos2d@@UAEXPBD@Z"),
				hooks::CCFileUtils_addSearchPath,
				&gates::CCFileUtils_addSearchPath,
				5
			}.enable();

			Hook{
				cocos2d::CCTexture2D::setDefaultAlphaPixelFormat,
				hooks::CCTexture2D_setDefaultAlphaPixelFormat,
				&gates::CCTexture2D_setDefaultAlphaPixelFormat,
				6
			}.enable();

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
			if (!HackProDelegate::sharedDelegate()) {
				Log::error("Could not find Mega Hack v6. Extension not created.");
			}
		}
		else {
		exit:
			Log::error("Could not find module base addresses. Exiting.");
			FreeLibraryAndExitThread(static_cast<HMODULE>(hModule), 0);
		}
	}
	catch (const std::exception& err) {
		Log::error("Initalization failed with error: ", err.what());
		MessageBox(nullptr, 
			"ERROR: Could not initialize. Contact the developer.",
			"textureldr", MB_OK | MB_ICONERROR);
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
		if (_) CloseHandle(_);
	}
	return TRUE;
}