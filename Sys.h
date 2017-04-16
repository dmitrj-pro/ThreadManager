/*
 * Sys.h
 *
 *  Created on: 7 апр. 2017 г.
 *      Author: diman-pro
 */

#ifndef SYS_H_
#define SYS_H_

#include<sstream>

#define LINUX

#include "MemoryManager/Memory.h"

using std::ostringstream;
using std::string;

template<typename T>
inline string toString(T N) {
	if (N==0)
		return "0";
	ostringstream os;
	os<<N;
    return os.str();
}

extern MemoryManager::Manager memory;

#endif /* SYS_H_ */
