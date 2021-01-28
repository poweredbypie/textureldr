#pragma once

#include "pch.h"

/*i didn't really want to make a static class here, but including this in pch.h
* really doesn't like it being a namespace, soo...
*/
class log {
private:
	static inline std::ofstream file;

public:
	static inline bool init() {
		using namespace std::filesystem;

		path log = current_path() / "packs/log.txt";

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
			MessageBox(0, "ERROR: log.txt is an existing directory.\n could not initialize logging.", "textureldr", MB_OK | MB_ICONERROR);
		}
		
		std::cout.rdbuf(file.rdbuf());

		if (file.good() && std::cout.rdbuf()) {
			if (file_size(log)) {
				std::cout << "\n\n";
			}
			log::info("Logger initialized.");
			return true;
		}
		return false;
	}

	template<typename ...Args>
	static inline void error(Args... args) {
		std::cout << "[ERROR] ";
		(std::cout << ... << args);
		std::cout << std::endl;
	}

	template<typename ...Args>
	static inline void info(Args... args) {
		std::cout << "[INFO] ";
		(std::cout << ... << args);
		std::cout << std::endl;
	}
};