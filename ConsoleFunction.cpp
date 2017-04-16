/*
 * ConsoleFunction.cpp
 *
 *  Created on: 7 апр. 2017 г.
 *      Author: diman-pro
 */

#include "ConsoleFunction.h"
#include <vector>
#include <chrono>

using std::string;
using Proces_MNG::console;
using SaveResultThread::Save;
using SaveResultThread::getSave;

inline void ps(){
	mng.runingWithName([](){
		console<<mng;
	},"ps");
}
inline void help(){
	mng.runingWithName([](){
		console<<"ps - список процессов\n"<<
				"help - справка\n"<<
				"exit - завершение программы\n"
				"sum - вычисление многопоточной суммы\n"
				#ifdef LINUX
					"kill - уничтожить поток\n"
				#endif
				"";
	},"help");
}
inline void sum(){
	int a=-100;
	int h=20;
	auto s=getSave(mng,[&a,h] (){
		int aa=(a+=h);
		int res=0;
		for (int i=aa-h;i<aa;i++){
			res+=i;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		return res;
	},10);
	s.run();
	s.weit();
	auto x=s.Result();
	console.lock_write();
	for (auto y=x.begin();y!=x.end();y++){
		console.write(*y).write(" ");
	}
	console.unlock_write();
}
inline void Myexit(){
	#ifdef LINUX
		mng.kill(0);
	#endif
}
#ifdef LINUX
inline void kill(){
	int id;
	console>>id;
	mng.runingWithName([id](){
		mng.kill(id);
	},"kill");
}
#endif


exec::Exec<string,void(*)()> GetAllFunction(){
	auto res=exec::getExec<string>("ps",ps);
	res.push("help",help);
	res.push("sum",sum);
	#ifdef LINUX
	res.push("kill",kill);
	res.push("exit",Myexit);
	#endif
	return res;
}
