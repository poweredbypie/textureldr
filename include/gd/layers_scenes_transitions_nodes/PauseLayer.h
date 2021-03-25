#ifndef __PAUSELAYER_H__
#define __PAUSELAYER_H__

#include <gd.h>

namespace gd {

	class CCBlockLayer;

	class PauseLayer : public CCBlockLayer {
	protected:
		bool m_bUnknown;
		bool m_bUnknown2;

	public:
		void createToggleButton(cocos2d::SEL_MenuHandler callback, bool on,
			cocos2d::CCMenu* menu, std::string caption, cocos2d::CCPoint pos) {
			return reinterpret_cast<void(__thiscall*)(PauseLayer*, cocos2d::SEL_MenuHandler,
				bool, cocos2d::CCMenu*, std::string, cocos2d::CCPoint)>(
					base + 0x1E5570
					)(this, callback, on, menu, caption, pos);
		}
	};
}

#endif