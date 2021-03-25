#include "pch.h"
#include "LoaderManager.h"

using namespace cocos2d;

#define ALL "All"
#define APPLIED "Applied"
#define QUALITY "Quality"

bool LoaderManager::init() {
	m_sFileName = "CCLoaderManager.dat";

	m_dAll = { ALL, 10 };
	m_dApplied = { APPLIED, 10 };
	m_dQuality = { QUALITY, 1 };

	this->setup();

	return true;
}

void LoaderManager::encodeListTo(DS_Dictionary* data, const ListData& list) {
	if (!data->stepIntoSubDictWithKey(list.m_sTitle)) {
		data->setSubDictForKey(list.m_sTitle);
		data->stepIntoSubDictWithKey(list.m_sTitle);
	}
	data->setStringArrayForKey("entries", list.m_vEntries);
	data->setIntegerForKey("offset", list.m_uOffset);
	data->setIntegerForKey("index", list.m_uIndex);
	data->stepOutOfSubDict();
}

bool LoaderManager::loadListFrom(DS_Dictionary* data, ListData& list) {
	if (!data->stepIntoSubDictWithKey(list.m_sTitle)) {
		return false;
	}
	list.m_vEntries = data->getStringArrayForKey("entries");
	list.m_uOffset = data->getIntegerForKey("offset");
	list.m_uIndex = data->getIntegerForKey("index");
	//handle error checking in GUI class.
		
	data->stepOutOfSubDict();

	return true;
}

void LoaderManager::encodeDataTo(DS_Dictionary* data) {
	data->setStringForKey("version", VERSION);

	this->encodeListTo(data, m_dAll);
	this->encodeListTo(data, m_dApplied);
	this->encodeListTo(data, m_dQuality);

	data->setBoolForKey("reloadSFX", m_bReloadSFX);
	data->setBoolForKey("reloadMusic", m_bReloadMusic);
	data->setBoolForKey("checkPlists", m_bCheckPlists);
	data->setBoolForKey("checkQuality", m_bCheckQuality);
	data->setBoolForKey("enhanceQuality", m_bEnhanceQuality);
}

void LoaderManager::dataLoaded(DS_Dictionary* data) {
	if (data->getStringForKey("version") != VERSION ||
		!this->loadListFrom(data, m_dAll) ||
		!this->loadListFrom(data, m_dApplied) ||
		!this->loadListFrom(data, m_dQuality)) {
		this->firstLoad();
		return;
	}

	m_bReloadSFX = data->getBoolForKey("reloadSFX");
	m_bReloadMusic = data->getBoolForKey("reloadMusic");
	m_bCheckPlists = data->getBoolForKey("checkPlists");
	m_bCheckQuality = data->getBoolForKey("checkQuality");
	m_bEnhanceQuality = data->getBoolForKey("enhanceQuality");
}

void LoaderManager::firstLoad() {
	this->updatePacks(false);
	m_dQuality.m_vEntries = { "Low", "Medium", "High" };
	m_dQuality.m_uOffset = 2;

	m_bReloadSFX = true;
	m_bReloadMusic = true;
	m_bCheckPlists = true;
	m_bCheckQuality = true;
	m_bEnhanceQuality = false;
}

void LoaderManager::syncVectors(const std::vector<std::string>& other, unsigned int& added, unsigned int& removed) {
	std::vector<std::string> old = m_dAll.m_vEntries;
	old.insert(old.end(), m_dApplied.m_vEntries.begin(), m_dApplied.m_vEntries.end());

	//add new packs not found in old ones
	for (unsigned int i = 0; i < other.size(); ++i) {
		if (std::find(old.begin(), old.end(), other[i]) == old.end()) {
			m_dAll.m_vEntries.insert(m_dAll.m_vEntries.begin(), other[i]);
			++added;
		}
	}

	//remove packs not found in new packs
	for (unsigned int i = 0; i < old.size(); ++i) {
		if (std::find(other.begin(), other.end(), old[i]) == other.end()) {
			auto index = std::find(m_dAll.m_vEntries.begin(), m_dAll.m_vEntries.end(), old[i]);
			if (index != m_dAll.m_vEntries.end()) {
				m_dAll.m_vEntries.erase(index);
			}
			else {
				index = std::find(m_dApplied.m_vEntries.begin(), m_dApplied.m_vEntries.end(), old[i]);
				m_dApplied.m_vEntries.erase(index);
			}

			//move to the start of the list, im too lazy to do calculation
			m_dAll.m_uIndex = 0;
			m_dAll.m_uOffset = 0;
			++removed;
		}
	}
}

bool LoaderManager::checkPlists() {
	using namespace std::filesystem;

	try {
		Log::info("Verifying applied pack plists.");
		/*checks for all plists in applied pack folders. then looks for pngs
		* that don't have corresponding plists, and sees if it's in the plists vector.
		* if so, and if the plist is at a higher index than the png, warn.
		* does not check for if plists are standalone; that is slower to do, and
		* a normal texture pack should always have a corresponding png for a modded
		* plist. otherwise it would always mess up lol
		*/
		std::vector<path> plists = {};

		for (auto packName : m_dApplied.m_vEntries) {
			path pack = current_path() / "packs" / packName;
			if (exists(pack)) {
				for (directory_entry file : directory_iterator{ pack }) {
					if (file.path().extension() == ".plist" ||
						file.path().extension() == ".fnt") {
						plists.push_back(file.path());
					}
				}
			}
		}
		for (unsigned int i = 0; i < m_dApplied.m_vEntries.size(); ++i) {
			path pack = current_path() / "packs" / m_dApplied.m_vEntries[i];
			if (exists(pack)) {
				for (directory_entry file : directory_iterator{ pack }) {
					if (file.path().extension() == ".png") {
						if (!exists(file.path().stem().replace_extension(".plist")) &&
							!exists(file.path().stem().replace_extension(".fnt"))) {
							auto index = std::find_if(plists.begin(), plists.end(), [file](path x) {
								return x.stem() == file.path().stem();
								});
							//check index of entry to see if the plist is below on the priority list
							if (index != plists.end()) {
								if (std::find(m_dApplied.m_vEntries.begin(),
									m_dApplied.m_vEntries.end(),
									plists[index - plists.begin()].parent_path().filename().string())
									- m_dApplied.m_vEntries.begin() > i) {
									return false;
								}
							}
						}

					}
				}
			}
		}
		return true;
	}
	catch (const std::exception& err) {
		Log::error("Plist verification failed with error: ", err.what());
		return true;
	}
}

bool LoaderManager::checkQuality() {
	using namespace std::filesystem;
	
	try {
		Log::info("Verifying applied pack qualities.");
		/*imo an invalid pack is one that has textures that have uhd variants,
		* that don't align with your current quality. if it just has like music
		* or textures that dont have uhd variants, i shouldn't warn you cuz then
		* it'd warn you like everytime lol
		*/
		bool valid = true;
		std::vector<std::string> pngs = {};
		path defaults = current_path() / "Resources";

		for (auto file : directory_iterator{ defaults }) {
			if (file.path().extension() == ".png") {
				auto filename = file.path().stem().string();
				if (filename.size() >= 3 &&
					filename.substr(filename.size() - 3) == "uhd") {
					pngs.push_back(filename.substr(0, filename.size() - 4));
				}
			}
		}
		for (auto packName : m_dApplied.m_vEntries) {
			path pack = current_path() / "packs" / packName;
			if (exists(pack)) {
				for (auto file : directory_iterator{ pack }) {
					if (file.path().extension() == ".png") {
						auto filename = file.path().stem().string();
						if (filename.size() >= 4 &&
							std::find_if(pngs.begin(), pngs.end(), [filename](std::string x) {
								return x == filename.substr(0, x.size());
							}) != pngs.end()) {
							switch (m_dQuality.m_uOffset) {
							case 0:
								if (filename.substr(filename.size() - 2) != "hd") return true;
								else valid = false;
								break;
							case 1:
								if (filename.substr(filename.size() - 3) == "-hd") return true;
								else valid = false;
								break;
							case 2:
								if (filename.substr(filename.size() - 4) == "-uhd") return true;
								else valid = false;
								break;
							}
						}
					}
				}
			}
		}

		return valid;
	}
	catch (const std::exception& err) {
		Log::error("Quality verification failed with error: ", err.what());
		return true;
	}
}

LoaderManager* LoaderManager::sharedState() {
	if (!s_sharedLoaderManager) {
		s_sharedLoaderManager = new LoaderManager();
		s_sharedLoaderManager->init();
	}

	return s_sharedLoaderManager;
}

std::string LoaderManager::updatePacks(bool generate) {
	using namespace std::filesystem;

	std::vector<std::string> packsList = {};

	/*all characters allowed by stock bigFont.fnt and goldFont.fnt.
	* excludes '\', '/', ':', '*', '?', '"', '<', '>', and '|' since windows
	* folders can't have those symbols, and '•' since it acts weird and i can't
	* be bothered lol
	*/
	constexpr std::string_view filter = " !#$%&'()+,-.0123456789;=@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{}~";
	unsigned int invalid = 0;

	Log::info("Searching for packs.");
	path packs = current_path() / "packs";
	if (exists(packs)) {
		if (is_directory(packs)) {
			Log::info("Packs directory found. Iterating.");
			for (directory_entry pack : directory_iterator{ packs }) {
				if (is_directory(pack)) {
					bool valid = true;
					for (auto c : pack.path().filename().u8string()) {
						if (filter.find(c) == filter.npos) {
							valid = false;
							break;
						}
					}

					if (valid)
						packsList.push_back(pack.path().filename().string());
					else
						++invalid;
				}
			}

			Log::info(packsList.size(), " packs found.");
		}
		else
			MessageBox(nullptr, "ERROR: packs is an existing file.\n please remove it to use textureldr.", "textureldr", MB_OK | MB_ICONERROR);
	}
	else {
		Log::error("No packs directory found. Creating new directory.");
		create_directories(packs);
	}

	unsigned int added = 0;
	unsigned int removed = 0;

	if (!(m_dAll.m_vEntries.empty() && m_dApplied.m_vEntries.empty())) {
		this->syncVectors(packsList, added, removed);
	}
	else {
		m_dAll.m_vEntries = packsList;
		added = packsList.size();
	}

	if (generate) {
		std::stringstream text;
		if (added != 0 || removed != 0) {
			text << "<cg>" << added << "</c> " << ((added == 1) ? "pack" : "packs") << " added.\n";
			//for some silly reason, color markers don't work directly after a newline!
			//@robtop fix???
			text << " <cr>" << removed << "</c> " << ((removed == 1) ? "pack" : "packs") << " removed.\n";
		}
		else {
			text << "<cy>Nothing changed!</c>\n";
		}

		if (invalid > 0) {
			if (invalid == 1) {
				text << "1 pack had an <co>invalid</c> name, and was ignored.\n";
			}
			else {
				text << invalid << " packs had <co>invalid</c> names, and were ignored.\n";
			}
		}
		return text.str();
	}
	else {
		return "";
	}
}