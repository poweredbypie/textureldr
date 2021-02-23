#ifndef __FLALERTLAYER_H__
#define __FLALERTLAYER_H__

#include <gd.h>

namespace gd {

	class ButtonSprite;
	class ScrollingLayer;

	#pragma runtime_checks("s", off)
	class FLAlertLayer : public cocos2d::CCLayerColor {
	protected:
		cocos2d::CCMenu* m_pButtonMenu;
		PAD(4);
		cocos2d::CCObject* m_pTarget;
		PAD(8);
		cocos2d::CCLayer* m_pLayer;
		PAD(4);
		bool m_bUnknown;
		ButtonSprite* m_pButton1;
		ButtonSprite* m_pButton2;
		ScrollingLayer* m_pScrollingLayer;
		PAD(4);
		bool m_bAlternateFormat; //?
		bool m_bSingleButton; //?

	public:
		/*FLAlertLayer supports colors of text for the caption. wrap desired text in "<cx></c>"
			* where x is the color desired. colors are:
			* r - red
			* l - lime
			* g - green
			* y - yellow
			* o - orange
			* and more that i'm too lazy to find.
			* MAKE SURE YOU FOLLOW THIS FORMAT. ROB'S PARSING CAN AND WILL MESS UP OTHERWISE.
			*/
		static FLAlertLayer* create(cocos2d::CCObject* target, const char* title,
			const char* btn1, const char* btn2, std::string caption) {
			static_assert(sizeof(std::string) == 24, "std::string in debug mode does not work correctly with FLAlertLayer!");
			auto pRet = reinterpret_cast<FLAlertLayer* (__fastcall*)(cocos2d::CCObject*, const char*,
				const char*, const char*, std::string)>(
					base + 0x22680
					)(target, title, btn1, btn2, caption);
			//clean stack.
			__asm add esp, 0x20
		}
		virtual void show() {}
	};
	#pragma runtime_checks("s", restore)
}


#endif