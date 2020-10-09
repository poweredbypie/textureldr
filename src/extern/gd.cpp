#include "pch.h"
#include "gd.h"

namespace gd {
	std::vector<std::pair<void*&, int>> fcnPtrInfo{};
	HMODULE hmodule{};

	namespace appDelegate {
		void(__thiscall* trySaveGame)(void* AppDelegate);

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)trySaveGame, 0x3D5E0 });
		}
	}
	namespace gamemanager {
		int(__thiscall* reloadAll)(void* GameManager, bool bSwitch, bool bFullscreen, bool bReloadedInSession);

		void* get() {
			return *(void**)((char*)hmodule + 0x3222D0);
		}
		void addTo() {
			fcnPtrInfo.push_back({ (void*&)reloadAll, 0xCE950 });
		}
	}
	namespace menuLayer {
		void** pMoreGamesStr;
		void** szMoreGamesBtn;
		void** pcbMoreGames;
		char* oMoreGamesStr;
		char* oFolderBtnStr;
		char* cbMoreGames;

		void* (__stdcall* create)();

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)create, 0x190550 });
		}
	}
	namespace loadingLayer {
		void(__thiscall* loadingFinished)(void* LoadingLayer);

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)loadingFinished, 0x18C790 });
		}
	}
	
	void addFunctions() {
		appDelegate::addTo();
		gamemanager::addTo();
		menuLayer::addTo();
		loadingLayer::addTo();
	}

	bool init() {
		addFunctions();

		//get offsets
		hmodule = GetModuleHandle(0);
		menuLayer::pMoreGamesStr = (void**)((char*)hmodule + 0x190EF2);
		menuLayer::szMoreGamesBtn = (void**)((char*)hmodule + 0x190F01);
		menuLayer::pcbMoreGames = (void**)((char*)hmodule + 0x190F13);
		menuLayer::oMoreGamesStr = ((char*)hmodule + 0x2CD790);
		menuLayer::oFolderBtnStr = ((char*)hmodule + 0x297B34);

		for (std::pair<void*&, int> pair : fcnPtrInfo) {
			pair.first = ((char*)hmodule + pair.second);
			if (!pair.first)
				return false;
		}
		return true;
	}
}