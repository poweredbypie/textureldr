#pragma once

#include "pch.h"

//every function pointer is inlined since it's constant and defined in this header.
#define DEFINE_AND_ASSIGN(name, type, addr) \
inline auto name = reinterpret_cast<type>(addr);

namespace gd {
	DEFINE_AND_ASSIGN(base, char*, GetModuleHandle(0));

	namespace appDelegate {
		DEFINE_AND_ASSIGN(trySaveGame, void(__thiscall const*)(void* AppDelegate), base + 0x3D5E0);
	}
	namespace gameManager {
		DEFINE_AND_ASSIGN(reloadAll, int(__thiscall const*)(void* GameManager, bool bSwitch, bool bFullscreen, bool bReloadedInSession), base + 0xCE950);

		inline void* get() {
			return *reinterpret_cast<void**>(base + 0x3222D0);
		}
	}
	namespace menuLayer {
		DEFINE_AND_ASSIGN(pMoreGamesStr, void const* const*, base + 0x190EF2);
		DEFINE_AND_ASSIGN(szMoreGamesBtn, float const* const*, base + 0x190F01);
		DEFINE_AND_ASSIGN(pcbMoreGames, void(__stdcall const*)(void* pSender), base + 0x190F13);
		DEFINE_AND_ASSIGN(oMoreGamesStr, char const*, base + 0x2CD790);
		DEFINE_AND_ASSIGN(oFolderBtnStr, char const*, base + 0x297B34);
		inline char* cbMoreGames = nullptr;

		DEFINE_AND_ASSIGN(create, void* (__stdcall const*)(), base + 0x190550);
		DEFINE_AND_ASSIGN(fadeInMusic, void(__stdcall const*)(const char* filename), base + 0xC4BD0);
	}
	namespace loadingLayer {
		DEFINE_AND_ASSIGN(loadingFinished, void(__thiscall const*)(void* LoadingLayer), base + 0x18C790);
	}

	inline bool init() {
		//if the base address is valid, all other values should be valid.
		return base;
	}
}

#undef DEFINE_AND_ASSIGN
