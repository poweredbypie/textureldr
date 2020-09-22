#include "pch.h"
#include "gd.h"

namespace gd {
	std::vector<std::pair<void*&, int>> fcnPtrInfo{};
	HMODULE hmodule{};

	namespace gamemanager {
		int* ptr;

		tReloadAll reloadAll;

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)reloadAll, oReloadAll });
		}
	}
	namespace menuLayer {
		void** pMoreGamesStr;
		void** szMoreGamesBtn;
		void** pcbMoreGames;
		char* oMoreGamesStr;
		char* oFolderBtnStr;
		char* cbMoreGames;
		tCreate create;

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)create, oCreate });
		}
	}
	namespace loadingLayer {
		tLoadingFinished loadingFinished;

		void addTo() {
			fcnPtrInfo.push_back({ (void*&)loadingFinished, oLoadingFinished });
		}
	}
	
	void addFunctions() {
		gamemanager::addTo();
		menuLayer::addTo();
		loadingLayer::addTo();
	}

	bool init() {
		addFunctions();

		//get offsets
		hmodule = GetModuleHandle(0);
		gamemanager::ptr = *(int**)((char*)hmodule + 0x3222D0);
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