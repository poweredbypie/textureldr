#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include <gd.h>

namespace gd {
	class GameManager : public GManager {
	protected:
		std::string m_sSaveFile;
		PAD(4);
		bool m_bSwitchModes;
		bool m_bToFullscreen;
		bool m_bReloading;

	public:
		static GameManager* sharedState() {
			return reinterpret_cast<GameManager* (__stdcall*)()>(
				base + 0xC4A50
				)();
		}
		void reloadAll(bool bSwitch, bool bFullscreen, bool bReloadedInSession) {
			return reinterpret_cast<void(__thiscall*)(GameManager*, bool, bool, bool)>(
				base + 0xCE950
				)(this, bSwitch, bFullscreen, bReloadedInSession);
		}
		void setQuality(cocos2d::TextureQuality quality) {
			*reinterpret_cast<cocos2d::TextureQuality*>(
				reinterpret_cast<char*>(this) + 0x2E4
				) = quality;
		}
	};
}

#endif