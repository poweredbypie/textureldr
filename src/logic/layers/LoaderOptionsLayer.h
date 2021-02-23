#ifndef __LOADEROPTIONSLAYER_H__
#define __LOADEROPTIONSLAYER_H__

#include "pch.h"

class LoaderOptionsLayer : public gd::GJDropDownLayer {
protected:


protected:
	virtual void customSetup();

public:
	static LoaderOptionsLayer* create();
};

#endif

