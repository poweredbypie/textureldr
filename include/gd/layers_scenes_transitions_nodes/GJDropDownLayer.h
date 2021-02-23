#ifndef __GJDROPDOWNLAYER_H__
#define __GJDROPDOWNLAYER_H__

#include <gd.h>

namespace gd {

	class GJListLayer;

	class GJDropDownLayer : public cocos2d::CCLayerColor {
	protected:
		cocos2d::CCPoint m_obEndPosition;
		cocos2d::CCPoint m_obStartPosition;
		cocos2d::CCMenu* m_pButtonMenu;
		GJListLayer* m_pListLayer;
		bool m_bControllerEnabled; //?
		cocos2d::CCLayer* m_pLayer;
		bool m_bHidden; //?
		PAD(4);

	public:
		virtual void customSetup() {}
		virtual void enterLayer() {}
		virtual void showLayer(bool noTransition) {}
		virtual void hideLayer(bool noTransition) {}
		virtual void layerVisible() {}
		virtual void layerHidden() {}
		virtual void enterAnimFinished() {}
		virtual void disableUI() {}
		virtual void enableUI() {}

		static GJDropDownLayer* create(const char* title) {
			GJDropDownLayer* pRet = new GJDropDownLayer();
			if (pRet) {
				pRet->constructor();
				if (pRet->init(title)) {
					pRet->autorelease();
				}
				//no idea if this works correctly, destructor calls vtable function with stack cookie or something
				else pRet->destructor();
			}
			return pRet;
		}
		void constructor() {
			return reinterpret_cast<void(__thiscall*)(GJDropDownLayer*)>(
				base + 0x038470
				)(this);
		}
		void destructor() {
			return reinterpret_cast<void(__thiscall*)(GJDropDownLayer*, bool)>(
				base + 0x038560
				)(this, true);
		}
		bool init(const char* title) {
			return reinterpret_cast<bool(__thiscall*)(GJDropDownLayer*, const char*)>(
				base + 0x113530
				)(this, title);
		}
	};
}

#endif