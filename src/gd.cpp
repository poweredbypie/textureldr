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
		void** cbGarage;
		void** cbLevelLayer;
		void** cbCreator;
		char* oBytesGarage;
		char* oBytesLevelLayer;
		char* oBytesCreator;

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
		menuLayer::cbGarage = (void**)((char*)hmodule + 0x12608A);
		menuLayer::cbLevelLayer = (void**)((char*)hmodule + 0x185DC7);
		menuLayer::cbCreator = (void**)((char*)hmodule + 0x4E8E3);


		for (std::pair<void*&, int> pair : fcnPtrInfo) {
			pair.first = ((char*)hmodule + pair.second);
			if (!pair.first)
				return false;
		}
		return true;
	}
}