// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#include "framework.h"
//vector for array of function pointers so initialization is less tedious, and packs list
#include <vector>
//for detecting and reading packs
#include <filesystem>
//for making strings
#include <sstream>
//for filter thing
#include <string_view>
//for logging
#include <iostream>
#include <fstream>
#include "log.h"

//for messagebox instantiation
#include <windows.h>

//game / engine headers
#include <cocos2d.h>
#include <cocos-ext.h>

#include <gd.h>

//version string
#define VERSION "v2.0a"

#endif //PCH_H
