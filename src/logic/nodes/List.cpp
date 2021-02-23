#include "pch.h"
#include "List.h"

using namespace cocos2d;

#define ARROW CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png")

enum {
	kListUpBtn,
	kListDownBtn,
	kVerticalListSwapUpBtn,
	kVerticalListSwapDownBtn,
	kVerticalListMoveBtn
};

//HorizontalList
void HorizontalList::navigate(cocos2d::CCObject* btn) {
	if (!m_vEntries.empty()) {
		if (btn->getTag() == kListUpBtn) {
			if (m_uOffset != 0)
				--m_uOffset;
		}
		else {
			if (m_uOffset < m_vEntries.size() - 1)
				++m_uOffset;
		}

		this->updateList();
	}
}

void HorizontalList::updateList() {
	this->getLength();

	if (m_uLength != 0) {
		if (!m_bEntered) {
			m_pLabels[0] = CCLabelBMFont::create(m_vEntries[m_uOffset].c_str(), "bigFont.fnt");
			m_pLabels[0]->setPosition(m_obPosition.x, m_obPosition.y);
			m_pMenu->addChild(m_pLabels[0]);
		}
		else {
			m_pLabels[0]->setString(m_vEntries[m_uOffset].c_str());
		}
		m_pLabels[0]->limitLabelWidth(90.0f, 0.8f, 0.0f);
	}
}

void HorizontalList::enter(LoaderLayer* parent) {
	m_pMenu = CCMenu::create();
	parent->addChild(m_pMenu);

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

	this->updateList();

	m_bEntered = true;
}

bool HorizontalList::load(tinyxml2::XMLDocument* file) {
	using namespace tinyxml2;

	if (auto list = file->FirstChildElement(m_sTitle)) {
		if (list->QueryUnsignedAttribute("offset", &m_uOffset))
			return false;

		for (auto i = list->FirstChildElement("entry");
			i;
			i = i->NextSiblingElement())
			m_vEntries.push_back(i->GetText());

		if (!m_vEntries.empty() && m_uOffset > m_vEntries.size() - 1)
			m_uOffset = m_vEntries.size() - 1;
		else if (m_uOffset < 0)
			m_uOffset = 0;

		return true;
	}
	return false;
}

bool HorizontalList::save(tinyxml2::XMLDocument* file) {
	using namespace tinyxml2;

	auto list = file->NewElement(m_sTitle);
	list->SetAttribute("offset", m_uOffset);
	file->InsertEndChild(list);
	for (auto entry : m_vEntries) {
		auto i = file->NewElement("entry");
		i->InsertEndChild(file->NewText(entry.c_str()));
		list->InsertEndChild(i);
	}

	return true;
}

HorizontalList::HorizontalList(const char* title) {
	m_sTitle = title;
	//m_uMaxLength is always 1 for horizontal lists
	m_pLabels = new CCLabelBMFont* [m_uMaxLength];
}

void HorizontalList::setEntries(const std::vector<std::string>& vec) {
	m_vEntries = vec;

	if (m_bEntered)
		this->updateList();
}

void HorizontalList::setOffset(unsigned int offset) {
	if (offset > m_vEntries.size() - 1) {
		Log::error("New offset is out of bounds.");
		return;
	}
	m_uOffset = offset;

	if (m_bEntered)
		this->updateList();
}

unsigned int HorizontalList::ifNotFound(const std::vector<std::string>& other, bool add) {
	unsigned int count = 0;

	if (add) {
		for (unsigned int i = 0; i < other.size(); ++i) {
			if (std::find(m_vEntries.begin(), m_vEntries.end(), other[i]) == m_vEntries.end()) {
				m_vEntries.insert(m_vEntries.begin(), other[i]);
				++count;
			}
		}
	}
	else {
		for (unsigned int i = 0; i < m_vEntries.size(); ++i) {
			if (std::find(other.begin(), other.end(), m_vEntries[i]) == other.end()) {
				m_vEntries.erase(m_vEntries.begin() + i);
				--i;

				//move to the start of the list, im too lazy
				m_uOffset = 0;
				++count;
			}
		}
	}

	if (m_bEntered)
		this->updateList();

	return count;
}

//VerticalList
void VerticalList::navigate(cocos2d::CCObject* btn) {
	if (btn->getTag() == kListUpBtn) {
		if (m_uIndex > 0)
			--m_uIndex;
		else if (m_uOffset > 0)
			--m_uOffset;
	}
	else {
		if (m_uIndex + 1 < m_uLength)
			++m_uIndex;
		else if (m_uMaxLength < m_vEntries.size() &&
			m_uOffset < m_vEntries.size() - m_uMaxLength)
			++m_uOffset;
	}

	updateList();
}

void VerticalList::swap(cocos2d::CCObject* btn) {
	if (btn->getTag() == kVerticalListSwapUpBtn) {
		if (m_uIndex > 0) {
			std::iter_swap(m_vEntries.begin() + m_uIndex + m_uOffset, m_vEntries.begin() + m_uIndex + m_uOffset - 1);
			--m_uIndex;
		}
	}
	else {
		if (m_uIndex + 1 < m_uLength) {
			std::iter_swap(m_vEntries.begin() + m_uIndex + m_uOffset, m_vEntries.begin() + m_uIndex + m_uOffset + 1);
			++m_uIndex;
		}
	}

	updateList();
}

void VerticalList::move(cocos2d::CCObject*) {
	m_pTarget->m_vEntries.insert(m_pTarget->m_vEntries.begin(), m_vEntries[m_uIndex + m_uOffset]);
	m_vEntries.erase(m_vEntries.begin() + m_uIndex + m_uOffset);

	getLength();

	if (m_vEntries.size() >= m_uMaxLength &&
		m_uLength < m_uMaxLength)
		--m_uOffset;
	else if (m_uLength < m_uIndex + 1 &&
		m_uLength > 0)
		--m_uIndex;

	updateList();
	m_pTarget->updateList();
}

void VerticalList::updateLabels() {
	getLength();
	if (!m_bEntered) {
		for (unsigned int i = 0; i < m_uMaxLength; ++i) {
			if (i < m_uLength) {
				m_pLabels[i] = CCLabelBMFont::create(m_vEntries[m_uOffset + i].c_str(), "bigFont.fnt");
			}
			else {
				m_pLabels[i] = CCLabelBMFont::create("", "bigFont.fnt");
			}
			m_pLabels[i]->setPosition(m_obPosition.x, m_obPosition.y - 20.0f * i);
			m_pLabels[i]->limitLabelWidth(95.0f, 0.6f, 0.0f);
			m_pMenu->addChild(m_pLabels[i]);
		}
	}
	else {
		for (unsigned int i = 0; i < m_uMaxLength; ++i) {
			if (i < m_uLength) {
				m_pLabels[i]->setString(m_vEntries[m_uOffset + i].c_str());
			}
			else {
				m_pLabels[i]->setString("");
			}
			m_pLabels[i]->limitLabelWidth(95.0f, 0.6f, 0.0f);
		}
	}
}

void VerticalList::updateSelector() {
	float mult = m_obPosition.x < 0 ? 55.0f : -55.0f;

	CCMenuItemSprite* moveBtn;
	CCMenuItemSprite* swapUpBtn;
	CCMenuItemSprite* swapDownBtn;

	if (!m_bEntered) {
		moveBtn = CCMenuItemSprite::create(
			ARROW,
			ARROW,
			nullptr,
			this,
			menu_selector(VerticalList::move)
		);
		moveBtn->setTag(kVerticalListMoveBtn);
		moveBtn->setRotation(mult == -55.0f ? 180.0f : 0.0f);
		moveBtn->setScale(0.5f);
		m_pMenu->addChild(moveBtn);

		swapUpBtn = CCMenuItemSprite::create(
			ARROW,
			ARROW,
			nullptr,
			this,
			menu_selector(VerticalList::swap)
		);
		swapUpBtn->setTag(kVerticalListSwapUpBtn);
		swapUpBtn->setRotation(-90.0f);
		swapUpBtn->setScale(0.5f);
		m_pMenu->addChild(swapUpBtn);

		swapDownBtn = CCMenuItemSprite::create(
			ARROW,
			ARROW,
			nullptr,
			this,
			menu_selector(VerticalList::swap)
		);
		swapDownBtn->setTag(kVerticalListSwapDownBtn);
		swapDownBtn->setRotation(90.0f);
		swapDownBtn->setScale(0.5f);
		m_pMenu->addChild(swapDownBtn);
	}
	else {
		moveBtn = static_cast<CCMenuItemSprite*>(m_pMenu->getChildByTag(kVerticalListMoveBtn));
		swapUpBtn = static_cast<CCMenuItemSprite*>(m_pMenu->getChildByTag(kVerticalListSwapUpBtn));
		swapDownBtn = static_cast<CCMenuItemSprite*>(m_pMenu->getChildByTag(kVerticalListSwapDownBtn));
	}

	if (m_vEntries.size() - m_uOffset == 0)
		toggle({ moveBtn, swapUpBtn, swapDownBtn }, false);
	else {
		moveBtn->setPosition(m_obPosition.x + mult, m_obPosition.y - 20.0f * m_uIndex);
		swapUpBtn->setPosition(m_obPosition.x - mult, m_obPosition.y + 5.0f - 20.0f * m_uIndex);
		swapDownBtn->setPosition(m_obPosition.x - mult, m_obPosition.y - 5.0f - 20.0f * m_uIndex);

		toggle({ moveBtn, swapUpBtn, swapDownBtn }, true);
	}
}

void VerticalList::enter(LoaderLayer* parent) {
	m_pMenu = CCMenu::create();
	parent->addChild(m_pMenu);

	auto title = CCLabelBMFont::create(m_sTitle, "goldFont.fnt");
	title->setPosition(m_obPosition.x, m_obPosition.y + 50.0f);
	m_pMenu->addChild(title);

	auto upBtn = gd::CCMenuItemSpriteExtra::create(
		ARROW,
		this,
		menu_selector(VerticalList::navigate)
	);
	upBtn->setTag(kListUpBtn);
	upBtn->setPosition(m_obPosition.x, m_obPosition.y + 25.0f);
	upBtn->setRotation(-90.0f);
	m_pMenu->addChild(upBtn);

	auto downBtn = gd::CCMenuItemSpriteExtra::create(
		ARROW,
		this,
		menu_selector(VerticalList::navigate)
	);
	downBtn->setTag(kListDownBtn);
	downBtn->setPosition(m_obPosition.x, m_obPosition.y - 205.0f);
	downBtn->setRotation(90.0f);
	m_pMenu->addChild(downBtn);

	this->updateList();

	m_bEntered = true;
}

bool VerticalList::load(tinyxml2::XMLDocument* file) {
	using namespace tinyxml2;

	if (auto list = file->FirstChildElement(m_sTitle)) {
		if (list->QueryUnsignedAttribute("offset", &m_uOffset) ||
			list->QueryUnsignedAttribute("index", &m_uIndex))
			return false;

		for (auto i = list->FirstChildElement("entry");
			i;
			i = i->NextSiblingElement())
			m_vEntries.push_back(i->GetText());

		if (m_uMaxLength < m_vEntries.size()) {
			if (m_uOffset > m_vEntries.size() - m_uMaxLength)
				m_uOffset = m_vEntries.size() - m_uMaxLength;
			else if (m_uOffset < 0)
				m_uOffset = 0;
		}
		else
			m_uOffset = 0;

		return true;
	}
	return false;
}

bool VerticalList::save(tinyxml2::XMLDocument* file) {
	using namespace tinyxml2;

	auto list = file->NewElement(m_sTitle);
	list->SetAttribute("offset", m_uOffset);
	list->SetAttribute("index", m_uIndex);
	file->InsertEndChild(list);
	for (auto entry : m_vEntries) {
		auto i = file->NewElement("entry");
		i->InsertEndChild(file->NewText(entry.c_str()));
		list->InsertEndChild(i);
	}

	return true;
}

VerticalList::VerticalList(const char* title, unsigned int length, VerticalList* target) {
	m_sTitle = title;
	m_uMaxLength = length;
	m_pTarget = target;

	m_pLabels = new CCLabelBMFont* [m_uMaxLength];
}

unsigned int VerticalList::ifNotFound(const std::vector<std::string>& other, bool add) {
	unsigned int count = 0;
	std::vector<std::string> old = { m_vEntries };
	old.insert(old.end(), m_pTarget->m_vEntries.begin(), m_pTarget->m_vEntries.end());

	if (add) {
		for (unsigned int i = 0; i < other.size(); ++i) {
			if (std::find(old.begin(), old.end(), other[i]) == old.end()) {
				m_vEntries.insert(m_vEntries.begin(), other[i]);
				++count;
			}
		}
	}
	else {
		for (unsigned int i = 0; i < old.size(); ++i) {
			if (std::find(other.begin(), other.end(), old[i]) == other.end()) {
				auto index = std::find(m_vEntries.begin(), m_vEntries.end(), old[i]);
				if (index != m_vEntries.end()) {
					m_vEntries.erase(index);
				}
				else {
					index = std::find(m_pTarget->m_vEntries.begin(), m_pTarget->m_vEntries.end(), old[i]);
					m_pTarget->m_vEntries.erase(index);
				}

				//move to the start of the list, im too lazy to do calculation
				m_uOffset = 0;
				m_uIndex = 0;
				++count;
			}
		}
	}

	if (m_bEntered)
		updateList();

	return count;
}

void VerticalList::setOffset(unsigned int offset) {
	//this isn't used; i just have it for consistency's sake.
	if (offset > m_vEntries.size() - m_uMaxLength - 1) {
		Log::error("New offset is out of bounds.");
		return;
	}

	m_uOffset = offset;
}