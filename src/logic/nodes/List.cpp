#include "pch.h"
#include "List.h"

using namespace cocos2d;
using namespace extension;

#define ARROW CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png")

enum {
	kListUpBtn,
	kListDownBtn,
	kVerticalListSwapUpBtn,
	kVerticalListSwapDownBtn,
	kVerticalListMoveBtn,
	kVerticalListBackground
};

//ListData
ListData::ListData(const char* title, unsigned int length, ListData* target) {
	m_sTitle = title;
	m_uMaxLength = length;
	m_pTarget = target;
	m_uLength = 0;
	m_uOffset = 0;
	m_uIndex = 0;
}

//HorizontalList
bool HorizontalList::init() {
	m_pLabels = new CCLabelBMFont* [m_pData.m_uMaxLength];

	m_pMenu = CCMenu::create();
	this->addChild(m_pMenu);

	auto upBtn = gd::CCMenuItemSpriteExtra::create(
		ARROW,
		this,
		menu_selector(HorizontalList::navigate)
	);
	upBtn->setTag(kListUpBtn);
	upBtn->setPosition(m_obPosition.x - 70.0f, m_obPosition.y);
	upBtn->setRotation(180.0f);
	m_pMenu->addChild(upBtn);

	auto downBtn = gd::CCMenuItemSpriteExtra::create(
		ARROW,
		this,
		menu_selector(HorizontalList::navigate)
	);
	downBtn->setTag(kListDownBtn);
	downBtn->setPosition(m_obPosition.x + 70.0f, m_obPosition.y);
	m_pMenu->addChild(downBtn);

	//verify offset
	if (!m_pData.m_vEntries.empty() && m_pData.m_uOffset > m_pData.m_vEntries.size() - 1)
		m_pData.m_uOffset = m_pData.m_vEntries.size() - 1;
	else if (m_pData.m_uOffset < 0)
		m_pData.m_uOffset = 0;

	this->getLength();

	if (m_pData.m_uLength != 0) {
		m_pLabels[0] = CCLabelBMFont::create(m_pData.m_vEntries[m_pData.m_uOffset].c_str(), "bigFont.fnt");
		m_pLabels[0]->setPosition(m_obPosition.x, m_obPosition.y);
		m_pLabels[0]->limitLabelWidth(90.0f, 0.8f, 0.0f);
		m_pMenu->addChild(m_pLabels[0]);
	}

	return true;
}

void HorizontalList::updateList() {
	this->getLength();

	if (m_pData.m_uLength != 0) {
		m_pLabels[0]->setString(m_pData.m_vEntries[m_pData.m_uOffset].c_str());
		m_pLabels[0]->limitLabelWidth(90.0f, 0.8f, 0.0f);
	}
}

void HorizontalList::navigate(cocos2d::CCObject* btn) {
	if (!m_pData.m_vEntries.empty()) {
		if (btn->getTag() == kListUpBtn) {
			if (m_pData.m_uOffset != 0)
				--m_pData.m_uOffset;
		}
		else if (btn->getTag() == kListDownBtn) {
			if (m_pData.m_uOffset < m_pData.m_vEntries.size() - 1)
				++m_pData.m_uOffset;
		}

		this->updateList();
	}
}

HorizontalList* HorizontalList::create(ListData& data) {
	HorizontalList* pRet = new HorizontalList(data);
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

void HorizontalList::setPosition(float x, float y) {
	m_obPosition = ccp(x, y);
	//this->updateList();
}

//VerticalList
void VerticalList::navigate(cocos2d::CCObject* btn) {
	if (btn->getTag() == kListUpBtn) {
		if (m_pData.m_uIndex > 0)
			--m_pData.m_uIndex;
		else if (m_pData.m_uOffset > 0)
			--m_pData.m_uOffset;
	}
	else if (btn->getTag() == kListDownBtn) {
		if (m_pData.m_uIndex + 1 < m_pData.m_uLength)
			++m_pData.m_uIndex;
		else if (m_pData.m_uMaxLength < m_pData.m_vEntries.size() &&
			m_pData.m_uOffset < m_pData.m_vEntries.size() - m_pData.m_uMaxLength)
			++m_pData.m_uOffset;
	}

	this->updateList();
}

void VerticalList::swap(cocos2d::CCObject* btn) {
	if (btn->getTag() == kVerticalListSwapUpBtn) {
		if (m_pData.m_uIndex > 0) {
			std::iter_swap(m_pData.m_vEntries.begin() + m_pData.m_uIndex + m_pData.m_uOffset,
				m_pData.m_vEntries.begin() + m_pData.m_uIndex + m_pData.m_uOffset - 1);
			--m_pData.m_uIndex;
		}
	}
	else if (btn->getTag() == kVerticalListSwapDownBtn) {
		if (m_pData.m_uIndex + 1 < m_pData.m_uLength) {
			std::iter_swap(m_pData.m_vEntries.begin() + m_pData.m_uIndex + m_pData.m_uOffset,
				m_pData.m_vEntries.begin() + m_pData.m_uIndex + m_pData.m_uOffset + 1);
			++m_pData.m_uIndex;
		}
	}

	this->updateList();
}

void VerticalList::move(cocos2d::CCObject*) {
	m_pData.m_pTarget->m_vEntries.insert(m_pData.m_pTarget->m_vEntries.begin(),
		m_pData.m_vEntries[m_pData.m_uIndex + m_pData.m_uOffset]);
	m_pData.m_vEntries.erase(m_pData.m_vEntries.begin() + m_pData.m_uIndex + m_pData.m_uOffset);

	this->getLength();

	if (m_pData.m_vEntries.size() >= m_pData.m_uMaxLength &&
		m_pData.m_uLength < m_pData.m_uMaxLength)
		--m_pData.m_uOffset;
	else if (m_pData.m_uLength < m_pData.m_uIndex + 1 &&
		m_pData.m_uLength > 0)
		--m_pData.m_uIndex;

	this->updateList();
	m_pTarget->updateList();
}

void VerticalList::updateLabels() {
	this->getLength();

	for (unsigned int i = 0; i < m_pData.m_uMaxLength; ++i) {
		if (i < m_pData.m_uLength) {
			m_pLabels[i]->setString(m_pData.m_vEntries[m_pData.m_uOffset + i].c_str());
		}
		else {
			m_pLabels[i]->setString("");
		}
		m_pLabels[i]->limitLabelWidth(95.0f, 0.6f, 0.0f);
	}
}

void VerticalList::updateSelector() {
	float mult = m_obPosition.x < 0 ? 55.0f : -55.0f;

	auto moveBtn = static_cast<CCMenuItemSprite*>(m_pMenu->getChildByTag(kVerticalListMoveBtn));
	auto swapUpBtn = static_cast<CCMenuItemSprite*>(m_pMenu->getChildByTag(kVerticalListSwapUpBtn));
	auto swapDownBtn = static_cast<CCMenuItemSprite*>(m_pMenu->getChildByTag(kVerticalListSwapDownBtn));

	if (m_pData.m_vEntries.size() - m_pData.m_uOffset == 0)
		toggle({ moveBtn, swapUpBtn, swapDownBtn }, false);
	else {
		moveBtn->setPosition(mult, -20.0f * m_pData.m_uIndex);
		swapUpBtn->setPosition(-mult, 5.0f - 20.0f * m_pData.m_uIndex);
		swapDownBtn->setPosition(-mult, -5.0f - 20.0f * m_pData.m_uIndex);

		toggle({ moveBtn, swapUpBtn, swapDownBtn }, true);
	}
}

bool VerticalList::init() {
	m_pLabels = new CCLabelBMFont* [m_pData.m_uMaxLength];

	m_pMenu = CCMenu::create();
	this->addChild(m_pMenu);

	auto bg = CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
	bg->setPosition(m_obPosition.x, m_obPosition.y - 90.0f);
	bg->setColor({ 0, 0, 0 });
	bg->setOpacity(75);
	bg->setContentSize({ 150.0f, 20.0f * m_pData.m_uMaxLength + 45.0f });
	bg->setTag(kVerticalListBackground);
	m_pMenu->addChild(bg);

	auto title = CCLabelBMFont::create(m_pData.m_sTitle, "goldFont.fnt");
	title->setPosition(m_obPosition.x, m_obPosition.y + 50.0f);
	m_pMenu->addChild(title);

	auto upBtn = gd::CCMenuItemSpriteExtra::create(
		ARROW,
		this,
		menu_selector(VerticalList::navigate)
	);
	upBtn->setTag(kListUpBtn);
	upBtn->setPosition(m_obPosition.x, m_obPosition.y + 20.0f);
	upBtn->setRotation(-90.0f);
	m_pMenu->addChild(upBtn);

	auto downBtn = gd::CCMenuItemSpriteExtra::create(
		ARROW,
		this,
		menu_selector(VerticalList::navigate)
	);
	downBtn->setTag(kListDownBtn);
	downBtn->setPosition(m_obPosition.x, m_obPosition.y - 200.0f);
	downBtn->setRotation(90.0f);
	m_pMenu->addChild(downBtn);

	if (m_pData.m_uMaxLength < m_pData.m_vEntries.size()) {
		if (m_pData.m_uOffset > m_pData.m_vEntries.size() - m_pData.m_uMaxLength)
			m_pData.m_uOffset = m_pData.m_vEntries.size() - m_pData.m_uMaxLength;
		else if (m_pData.m_uOffset < 0)
			m_pData.m_uOffset = 0;
	}
	else {
		m_pData.m_uOffset = 0;
	}

	this->getLength();

	for (unsigned int i = 0; i < m_pData.m_uMaxLength; ++i) {
		if (i < m_pData.m_uLength) {
			m_pLabels[i] = CCLabelBMFont::create(m_pData.m_vEntries[m_pData.m_uOffset + i].c_str(), "bigFont.fnt");
		}
		else {
			m_pLabels[i] = CCLabelBMFont::create("", "bigFont.fnt");
		}
		m_pLabels[i]->setPosition(m_obPosition.x, m_obPosition.y - 20.0f * i);
		m_pMenu->addChild(m_pLabels[i]);
	}

	this->updateLabels();

	auto moveSprite = ARROW;
	moveSprite->setScale(0.5f);
	auto moveBtn = gd::CCMenuItemSpriteExtra::create(
		moveSprite,
		this,
		menu_selector(VerticalList::move)
	);
	moveBtn->setTag(kVerticalListMoveBtn);
	m_pMenu->addChild(moveBtn);

	auto swapUpSprite = ARROW;
	swapUpSprite->setScale(0.5f);
	auto swapUpBtn = gd::CCMenuItemSpriteExtra::create(
		swapUpSprite,
		this,
		menu_selector(VerticalList::swap)
	);
	swapUpBtn->setTag(kVerticalListSwapUpBtn);
	swapUpBtn->setRotation(-90.0f);
	m_pMenu->addChild(swapUpBtn);

	auto swapDownSprite = ARROW;
	swapDownSprite->setScale(0.5f);
	auto swapDownBtn = gd::CCMenuItemSpriteExtra::create(
		swapDownSprite,
		this,
		menu_selector(VerticalList::swap)
	);
	swapDownBtn->setTag(kVerticalListSwapDownBtn);
	swapDownBtn->setRotation(90.0f);
	m_pMenu->addChild(swapDownBtn);

	this->updateSelector();

	return true;
}

VerticalList* VerticalList::create(ListData& data) {
	VerticalList* pRet = new VerticalList(data);
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

void VerticalList::setPosition(float x, float y) {
	m_obPosition = ccp(x, y);
	auto moveBtn = static_cast<CCMenuItemSprite*>(m_pMenu->getChildByTag(kVerticalListMoveBtn));
	moveBtn->setRotation(m_obPosition.x > 0 ? 180.0f : 0.0f);

	this->updateList();
}