#ifndef __LOG_H__
#define __LOG_H__

#include "pch.h"

/*i didn't really want to make a static class here, but including this in pch.h
* really doesn't like it being a namespace, soo...
*/
class Log {
private:
	static inline std::ofstream file;
	static inline bool enabled = false;

public:
	static inline bool init() {
		using namespace std::filesystem;

		path packs = current_path() / "packs";
		path log = packs / "log.txt";

		if (!exists(packs)) {
			create_directories(packs);
		}
		else if (!is_directory(packs)) {
			MessageBox(nullptr, "ERROR: packs is an existing file.\n please remove it to use textureldr.", "textureldr", MB_OK | MB_ICONERROR);
		}

		if (!is_directory(log)) {
			if (exists(log) && file_size(log) > 500000) {
				//if the file is over 0.5MB, overwrite it.
				file.open(log.c_str(), std::ios::trunc);
			}
			else {
				//if the file isn't over, or doesn't exist, open in append mode.
				file.open(log.c_str(), std::ios::app);
			}
		}
		else {
			MessageBox(nullptr, "ERROR: log.txt is an existing directory.\n could not initialize logging.", "textureldr", MB_OK | MB_ICONERROR);
		}
		
		std::cout.rdbuf(file.rdbuf());

		if (file.good() && std::cout.rdbuf()) {
			if (file_size(log)) {
				std::cout << "\n\n";
			}
			Log::info("Logger initialized.");
			enabled = true;
			return true;
		}
		return false;
	}

	template<typename ...Args>
	static inline void error(Args... args) {
		if (enabled) {
			std::cout << "[ERROR] ";
			(std::cout << ... << args);
			std::cout << std::endl;
		}
	}

	template<typename ...Args>
	static inline void info(Args... args) {
		if (enabled) {
			std::cout << "[INFO] ";
			(std::cout << ... << args);
			std::cout << std::endl;
		}
	}
};

#endif