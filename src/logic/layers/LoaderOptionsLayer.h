#ifndef __LOADEROPTIONSLAYER_H__
#define __LOADEROPTIONSLAYER_H__

#include "pch.h"

class LoaderOptionsLayer : public gd::GJDropDownLayer {
	//virtual void customSetup() override;

	static LoaderOptionsLayer* create(const char* title);
};

#endif

