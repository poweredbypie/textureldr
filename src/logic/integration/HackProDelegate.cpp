#include "pch.h"
#include "HackProDelegate.h"

using namespace cocos2d;
using namespace Hackpro;

HackProDelegate* HackProDelegate::create() {
	return static_cast<HackProDelegate*>(Extension::Create("textureldr"));
}

void HackProDelegate::init() {
	this->Commit();
}

HackProDelegate* HackProDelegate::sharedDelegate() {
	if (!s_pSharedHackProDelegate) {
		if (!Hackpro::Initialise()) return nullptr;
		while (!Hackpro::IsReady()) Sleep(50);
		s_pSharedHackProDelegate = HackProDelegate::create();
		s_pSharedHackProDelegate->init();
	}

	return s_pSharedHackProDelegate;
}