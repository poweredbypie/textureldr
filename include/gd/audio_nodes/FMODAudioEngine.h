#ifndef __FMODAUDIOENGINE_H__
#define __FMODAUDIOENGINE_H__

#include <gd.h>

namespace gd {
	class FMODAudioEngine : public cocos2d::CCNode {
	protected:
		cocos2d::CCDictionary* m_pDictionary;
		std::string m_sFilePath;
		float m_fBackgroundMusicVolume;
		float m_fEffectsVolume;
		float m_fPulse1;
		float m_fPulse2;
		float m_fPulse3;
		int m_nPulseCounter;
		bool m_bMetering;
		bool m_bFading;
		bool m_bFadeIn;
		float m_fFadeInDuration;
		void* /*FMOD::Sound*/ m_pSound;
		void* /*FMOD::ChannelGroup*/ m_pChannelGroup;
		bool m_bPaused;
		bool* m_pbBackgroundMusicPlaying;
		void* /*FMOD::FMOD_DSP_METERING_INFO*/ m_pMeteringInfo;
		int /*FMOD::FMOD_RESULT*/ m_eLastResult;
		PAD(0x8);
		int m_nMusicOffset;

	public:
		static FMODAudioEngine* sharedEngine() {
			return reinterpret_cast<FMODAudioEngine * (__stdcall*)()>(
				base + 0x239F0
				)();
		}
		void preloadEffect(std::string filename) {
			return reinterpret_cast<void(__thiscall*)(FMODAudioEngine*, std::string)>(
				base + 0x24240
				)(this, filename);
		}
		//my own function
		void reloadEffects() {
			using namespace std::filesystem;

			std::vector<std::string> oggs = {};
			path defaults = current_path() / "Resources";
			for (auto file : directory_iterator{ defaults }) {
				if (file.path().extension() == ".ogg")
					oggs.push_back(file.path().filename().string());
			}
			for (auto ogg : oggs) {
				m_pDictionary->removeObjectForKey(ogg);
				this->preloadEffect(ogg);
			}
		}
	};
}

#endif