#ifndef __LIST_H__
#define __LIST_H__

#include "pch.h"
#include "../layers/LoaderLayer.h"

class LoaderLayer;

class HorizontalList : public cocos2d::CCNode {
protected:
	const char* m_sTitle = "";
	std::vector<std::string> m_vEntries = {};
	cocos2d::CCLabelBMFont** m_pLabels = nullptr;
	unsigned int m_uMaxLength = 1;
	unsigned int m_uLength = 0;
	unsigned int m_uOffset = 0;

	cocos2d::CCMenu* m_pMenu = nullptr;

	bool m_bEntered = false;

protected:
	HorizontalList() = default;

	inline void getLength() {
		m_uLength = m_vEntries.size() - m_uOffset < m_uMaxLength ?
			(m_vEntries.size() - m_uOffset)
			: m_uMaxLength;
	}
	inline void toggle(std::vector<cocos2d::CCMenuItemSprite*> btns, bool enable) {
		for (auto btn : btns) {
			btn->setVisible(enable);
			btn->setEnabled(enable);
		}
	}

public:
	virtual void navigate(cocos2d::CCObject* btn);

	virtual void updateList();

	virtual void enter(LoaderLayer* parent);
	inline void exit() { 
		this->removeAllChildren();
		m_pMenu = nullptr;
		m_bEntered = false; 
	}

	HorizontalList(const char* title);

	virtual bool load(tinyxml2::XMLDocument* file);
	virtual bool save(tinyxml2::XMLDocument* file);

	void setEntries(const std::vector<std::string>& vec);
	virtual void setOffset(unsigned int offset);
	virtual unsigned int ifNotFound(const std::vector<std::string>& other, bool add);

	const inline unsigned int getIndex() { return m_uOffset; };
	const inline std::vector<std::string>& getEntries() { return m_vEntries; };
};

class VerticalList : public HorizontalList {
protected:
	unsigned int m_uIndex = 0;
	VerticalList* m_pTarget;

protected:
	virtual void navigate(cocos2d::CCObject* btn) override;
	void swap(cocos2d::CCObject* btn);
	void move(cocos2d::CCObject*);

	void updateLabels();
	void updateSelector();
	virtual inline void updateList() override {
		updateLabels();
		updateSelector();
	}

public:
	virtual void enter(LoaderLayer* parent) override;

	VerticalList(const char* title, unsigned int length, VerticalList* target);
	
	virtual bool load(tinyxml2::XMLDocument* file) override;
	virtual bool save(tinyxml2::XMLDocument* file) override;

	virtual void setOffset(unsigned int offset) override;

	virtual unsigned int ifNotFound(const std::vector<std::string>& other, bool add) override;
};

#endif