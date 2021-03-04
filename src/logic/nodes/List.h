#ifndef __LIST_H__
#define __LIST_H__

#include "pch.h"

class LoaderLayer;

struct ListData {
	const char* m_sTitle;
	std::vector<std::string> m_vEntries;
	unsigned int m_uMaxLength;
	unsigned int m_uLength;
	unsigned int m_uOffset;
	unsigned int m_uIndex;
	ListData* m_pTarget;

	ListData(const char* title, unsigned int length, ListData* target);
	ListData() { /*doesn't matter since only used in default ctor of LoaderManager*/ };
};

class HorizontalList : public cocos2d::CCNode {
protected:
	ListData& m_pData;
	cocos2d::CCLabelBMFont** m_pLabels = nullptr;
	cocos2d::CCMenu* m_pMenu = nullptr;

protected:
	HorizontalList(ListData& data) : m_pData{ data } {}
	virtual ~HorizontalList() { delete[] m_pLabels; }

	virtual bool init();

	inline void getLength() {
		m_pData.m_uLength = m_pData.m_vEntries.size() - 
			m_pData.m_uOffset < m_pData.m_uMaxLength ?
			(m_pData.m_vEntries.size() - m_pData.m_uOffset)
			: m_pData.m_uMaxLength;
	}
	inline void toggle(std::vector<cocos2d::CCMenuItemSprite*> btns, bool enable) {
		for (auto btn : btns) {
			btn->setVisible(enable);
			btn->setEnabled(enable);
		}
	}
	virtual void navigate(cocos2d::CCObject* btn);

public:
	static HorizontalList* create(ListData& data);

	virtual void updateList();
	virtual void setPosition(float x, float y);
};

class VerticalList : public HorizontalList {
	VerticalList* m_pTarget;

protected:
	VerticalList(ListData& data) : HorizontalList(data) {}

	virtual bool init();

	virtual void navigate(cocos2d::CCObject* btn) override;
	void swap(cocos2d::CCObject* btn);
	void move(cocos2d::CCObject*);

	void updateLabels();
	void updateSelector();

public:
	static VerticalList* create(ListData& data);

	virtual inline void updateList() override {
		updateLabels();
		updateSelector();
	}
	virtual void setPosition(float x, float y);

	void setTarget(VerticalList* target) { m_pTarget = target; }
};

#endif