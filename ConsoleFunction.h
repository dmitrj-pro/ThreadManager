/*
 * ConsoleFunction.h
 *
 *  Created on: 7 апр. 2017 г.
 *      Author: diman-pro
 */

#ifndef CONSOLEFUNCTION_H_
#define CONSOLEFUNCTION_H_

#include <string>
#include "ProcessManager/MultiThreadOut.h"
#include "ProcessManager/ProcessManager.h"
#include "SaveResult/SaveResult.h"
#include "MasterExcecute/Exec.h"

using std::string;

extern Proces_MNG::ProcessManager mng;

exec::Exec<string,void(*)()> GetAllFunction();

#endif /* CONSOLEFUNCTION_H_ */
