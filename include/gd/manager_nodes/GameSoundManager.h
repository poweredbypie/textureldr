#ifndef __GAMESOUNDMANAGER_H__
#define __GAMESOUNDMANAGER_H__

#include <gd.h>
#include <filesystem>

namespace gd {
	class GameSoundManager : public cocos2d::CCNode {
	protected:
		cocos2d::CCDictionary* m_pDictionary1;
		cocos2d::CCDictionary* m_pDictionary2;
		PAD(12);
		bool m_bPreloaded;
		PAD(4);
		std::string m_sFilePath;

	public:
		static GameSoundManager* sharedState() {
			return reinterpret_cast<GameSoundManager* (__stdcall*)()>(
				base + 0x24800
				)();
		}
	};
}

#endif