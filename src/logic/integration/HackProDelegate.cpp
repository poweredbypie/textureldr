#include "pch.h"
#include "HackProDelegate.h"

using namespace cocos2d;
using namespace Hackpro;

HackProDelegate* HackProDelegate::create() {
	return static_cast<HackProDelegate*>(Extension::Create("textureldr"));
}

void HackProDelegate::init() {
	this->AddButton("Reload Textures", callback_func(HackProDelegate::reloadAll));
	//const char* strs[] = { "1", "2", "3", nullptr };
	//this->AddComboBox(combobox_callback_func(HackProDelegate::sdfd))->SetStrings(strs);
	//this->AddTextBox(callback_func(HackProDelegate::d))->SetPlaceholder("hi");
	//this->AddCheckbox("h", callback_func(HackProDelegate::d), callback_func(HackProDelegate::d));
	this->Commit();
}

void HackProDelegate::reloadAll() {
	gd::GameManager::sharedState()->reloadAll(false, false, false);
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