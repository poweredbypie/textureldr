#ifndef __LOADERMANAGER_H__
#define __LOADERMANAGER_H__

#include "pch.h"
#include "List.h"

class LoaderManager : public gd::GManager {
protected:
	static inline LoaderManager* s_sharedLoaderManager = nullptr;

public:
	ListData m_dAll;
	ListData m_dApplied;
	ListData m_dQuality;
	bool m_bReloadSFX;
	bool m_bReloadMusic;
	bool m_bCheckPlists;
	bool m_bCheckQuality;
	bool m_bEnhanceQuality;

protected:
	virtual bool init();

	virtual void encodeDataTo(DS_Dictionary* data);
	virtual void dataLoaded(DS_Dictionary* data);
	virtual void firstLoad();

	void syncVectors(const std::vector<std::string>& other, unsigned int& added, unsigned int& removed);

	inline void encodeListTo(DS_Dictionary* data, const ListData& list);
	inline bool loadListFrom(DS_Dictionary* data, ListData& list);

public:
	static LoaderManager* sharedState();

	std::string updatePacks(bool generate);
	bool checkPlists();
	bool checkQuality();
};

#endif