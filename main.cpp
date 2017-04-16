//============================================================================
// Name        : ProcessManager.cpp
// Author      : Diman PRO
// Version     :
// Copyright   : All reserved
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <string>
#include "ProcessManager/ProcessManager.h"
#include "ProcessManager/MultiThreadOut.h"
#include "ConsoleFunction.h"

#include "MasterExcecute/Exec.h"

#include <unistd.h>


//Запуск тестирования из 2 задания
//#include "Generator/MasterGen.h"

using std::string;
using Proces_MNG::ProcessManager;
using Proces_MNG::console ;

ProcessManager mng;

//Запуск тестирования из 2 задания
/*void b(){
	int res;
	console>>res;
	mng.runingWithName([res](){
		Generator::Collection::Master gen;
		gen.SetValue("i",30);
		gen.SetValue("bgg");
		console<<gen(res);
	},"generator");
}*/

//Запуск тестирования из 2 задания
//void GenTest();

//void test(){
//	mng.runingWithName(GenTest,"Test Generator");
//}

int main() {
	console<<"run\n";
	auto commands=GetAllFunction();
	//Запуск тестирования из 2 задания
	//commands.push("gen",b);
	//commands.push("test",test);


	while(true){
		string str;
		console>>str;
		if (!commands.contains(str))
			console<<"Command is not found\n";
		commands.exc(str);
		if (str=="exit")
			break;
	}

	console<<"Success";
}
