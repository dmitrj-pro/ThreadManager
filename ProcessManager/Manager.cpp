/*
 * Manager.cpp
 *
 *  Created on: 7 апр. 2017 г.
 *      Author: diman-pro
 */

#include <iostream>
#include "MultiThreadOut.h"
#include "Process.h"
#include "ProcessManager.h"
#include "../Sys.h"

using std::istream;
using std::cout;
using std::ostream;
using std::string;
using std::cin;
using Proces_MNG::Multi_Process_Out;

namespace Proces_MNG{
	Multi_Process_Out<istream,ostream> console{cin,cout};

	string ToString(const ProcessStatus& st){
		switch(st){
			case ProcessStatus::Fail:
				return "Fail";
			case ProcessStatus::NIL:
				return "Fatal Error";
			case ProcessStatus::NotRunning:
				return "Not running";
			case ProcessStatus::Running:
				return "Running";
			case ProcessStatus::Succes:
				return "Success";
			default:
				return "System Failure";
		}
	}

	void ProcessManager::clear(){
		lock_process();
		bool t=false;
		auto y=_th.begin();
		auto x=_all.begin();
		for (;x!=_all.end();x++){
			if ((*x)->status()==ProcessStatus::Succes){
				t=true;
				break;
			}
			if ((*x)->status()==ProcessStatus::Fail){
				string out="Fail running ";
				out+=(*x)->Info().Name();
				out+=". I try running ";
				out+=toString((*x)->MaxTryRunning)+"\n";
				console<<out;
				t=true;
				break;
			}
			y++;

		}
		if (t){
			TypeProcess* tmp=(*x);
			_all.erase(x);
			delete tmp;
			thread* t= (*y);
			//if (!t->joinable())
			t->join();
			delete t;
			_th.erase(y);
		}
		unlock_process();
	}
	#ifdef LINUX
	void ProcessManager::kill(int id){
		lockAddProcess.lock();
		bool t=false;
		auto y=_th.begin();
		auto x=_all.begin();
		for (;x!=_all.end();x++){
			if ((*x)->Info().ID()==id){
				t=true;
				break;
			}
			y++;

		}
		if (t){
			TypeProcess* tmp=(*x);
			_all.erase(x);
			delete tmp;
			thread* t= (*y);
			pthread_cancel(t->native_handle());
			_th.erase(y);
		}

		lockAddProcess.unlock();
	}
	#endif
	void ProcessManager::weit(int id){
		lock_process();
		auto x=_all.begin();
		auto y=_th.begin();
		for (;x!=_all.end();x++){
			if ((*x)->Info().ID()==id){
				(*y)->join();
				TypeProcess* tmp=(*x);
				_all.erase(x);
				delete tmp;
				thread* t= (*y);
				delete t;
				_th.erase(y);
				break;
			}
			y++;
		}
		unlock_process();
	}
	void ProcessManager::weitAll(){
		lock_process();
		auto y=_th.begin();
		auto x=_all.begin();
		y++;
		for (;y!=_th.end();y++){
			(*y)->join();
			TypeProcess* tmp=(*x);
			_all.erase(x);
			delete tmp;
			thread* t= (*y);
			delete t;
			_th.erase(y);
			x++;
		}
		unlock_process();
	}
	void ProcessManager::weitAll(const vector<int>& ids){
		lock_process();
		auto x=_all.begin();
		auto y=_th.begin();
		for (;x!=_all.end();){
			bool flag=false;
			for (auto z=0;z!=ids.size();z++){
				if ((*x)->Info().ID()==ids[z]){
					flag=true;
					TypeProcess* tmp=(*x);
					_all.erase(x);
					delete tmp;
					thread* t= (*y);
					(*t).join();
					delete t;
					_th.erase(y);
					break;
				}
			}
			if (flag)
				continue;
			x++;
			y++;
		}
		unlock_process();
	}
}

