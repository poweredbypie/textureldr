#ifndef __LOADERLAYER_H__
#define __LOADERLAYER_H__

#include "pch.h"
#include "../nodes/List.h"

class HorizontalList;
class VerticalList;

class LoaderLayer : public cocos2d::CCLayer {
private:
	static HorizontalList s_lQuality;
	static VerticalList s_lAll, s_lApplied;
	static inline tinyxml2::XMLDocument* s_pFile = nullptr;
	static inline constexpr char s_sFilePath[] = "packs/config.dat";
	static inline constexpr char s_sBackupPath[] = "packs/backup.dat";

private:
	virtual bool init() override;

	static LoaderLayer* create();

	virtual void keyBackClicked() override;

	void onApply(cocos2d::CCObject*);
	void onOptions(cocos2d::CCObject*);
	void onExit(cocos2d::CCObject*);

	void getPacks(cocos2d::CCObject*);

	static std::string getPacks(bool generate);

public:
	static bool load();
	static bool save();

	static const std::vector<std::string>& getApplied();

	friend bool init();
	friend void __stdcall enter(cocos2d::CCObject*);
};

bool init();
void __stdcall enter(cocos2d::CCObject*);

#endif

