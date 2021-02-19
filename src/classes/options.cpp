#include "pch.h"
#include "options.h"

void options::enter(void* pSender) {
	using namespace gd;
	using namespace cocos2d;

	m_layer = GJDropDownLayer::create("Options");
	static_cast<CCNode*>(pSender)->getParent()->addChild(m_layer);



	auto winSize = CCDirector::sharedDirector()->getWinSize();
	m_layer->setPosition(-winSize.width / 2, -winSize.height / 2);
	m_layer->showLayer(false);
}
