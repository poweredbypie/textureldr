#include "pch.h"
#pragma once
#define FN_T using
#define OFFSET static inline const int

namespace gd {
	extern std::vector<std::pair<void*&, int>> fcnPtrInfo;
	extern HMODULE hmodule;

	namespace gamemanager {
		extern int* ptr;

		FN_T tReloadAll = int(__thiscall*)(void* GameManager, bool bSwitch, bool bFullscreen, bool bReloadedInSession);

		OFFSET oReloadAll = 0xCE950;

		extern tReloadAll reloadAll;

		void addTo();
	}
	namespace menuLayer {
		extern void** cbGarage;
		extern void** cbLevelLayer;
		extern void** cbCreator;

		extern char* oBytesGarage;
		extern char* oBytesLevelLayer;
		extern char* oBytesCreator;

		FN_T tCreate = void* (__stdcall*)();

		OFFSET oCreate = 0x190550;

		extern tCreate create;

		void addTo();
	}
	namespace loadingLayer {
		FN_T tLoadingFinished = void(__thiscall*)(void* LoadingLayer);

		OFFSET oLoadingFinished = 0x18C790;
		
		extern tLoadingFinished loadingFinished;

		void addTo();
	}

	void addFunctions();

	bool init();
}
