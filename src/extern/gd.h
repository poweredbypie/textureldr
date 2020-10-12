#pragma once

#include "pch.h"

namespace gd {
	extern std::vector<std::pair<void*&, int>> fcnPtrInfo;
	extern HMODULE hmodule;

	namespace appDelegate {
		extern void(__thiscall* trySaveGame)(void* AppDelegate);
	}
	namespace gamemanager {
		extern int(__thiscall* reloadAll)(void* GameManager, bool bSwitch, bool bFullscreen, bool bReloadedInSession);

		void* get();
	}
	namespace menuLayer {
		extern void** pMoreGamesStr;
		extern void** szMoreGamesBtn;
		extern void** pcbMoreGames;
		extern char* oMoreGamesStr;
		extern char* oFolderBtnStr;
		extern char* cbMoreGames;

		extern void* (__stdcall* create)();
	}
	namespace loadingLayer {
		extern void(__thiscall* loadingFinished)(void* LoadingLayer);
	}

	bool init();
}
