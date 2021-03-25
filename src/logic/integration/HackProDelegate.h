#ifndef __HACKPRODELEGATE_H__
#define __HACKPRODELEGATE_H__

#include "pch.h"
#include <hackpro_ext.hpp>
#include "../nodes/LoaderManager.h"

class HackProDelegate : public Hackpro::Extension {
protected:
	static inline HackProDelegate* s_pSharedHackProDelegate = nullptr;

protected:
	static HackProDelegate* create();
	void init();

	void __stdcall reloadAll();

public:
	static HackProDelegate* sharedDelegate();
};

#endif