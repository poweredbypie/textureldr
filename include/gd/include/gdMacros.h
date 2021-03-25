#ifndef __GDMACROS_H__
#define __GDMACROS_H__

/** PAD
* Add padding to a class / struct. For shifting classes /
* structs to be aligned, if too lazy to fully reverse.
*
* Based on line number, to be standard C / C++ compatible.
*/
#define PAD(size) char __STR_CAT__(pad, __LINE__)[size] = {};

#endif