/*
 * ProcessManager.h
 *
 *  Created on: 6 апр. 2017 г.
 *      Author: diman-pro
 */

#ifndef PROCESSMANAGER_PROCESSMANAGER_H_
#define PROCESSMANAGER_PROCESSMANAGER_H_

#include <thread>
#include <ostream>
#include <string>
#include "Process.h"


#include "../Sys.h"
#include "sys.h"
#include <mutex>
#include <chrono>

#ifdef LINUX
#include <unistd.h>
#include <pthread.h>
#endif

#define GloBalMemoryManager memory

using std::thread;
using std::ostream;
using std::vector;
using std::string;

typedef std::mutex MUTEX;


namespace Proces_MNG{
	typedef Process TypeProcess;

	//Запуск потока.
	//FUNC - запускаемая функция, ничего не принимающая
	//HANDLE - функция, запускаемая после запуска FUNC. На вход должна принимать результат первой
	//ERROR - функция, обрабатывающая ошибку при работе FUNC или HANDLE
	template <typename FUNC, typename HANDLE,typename ERROR>
	void run(TypeProcess *p,FUNC f,HANDLE tr,ERROR er){
		p->run(f,tr,er);
	}
	//запуск потока с функцией FUNC
	template <typename FUNC>
	void easyRun(TypeProcess *p,FUNC f){
		p->run(f);
	}

	class ProcessManager{
		private:

			//Следующий id запускаемого потока
			int& id;
			//Все потоки
			vector<thread*>& _th;
			//информация обо всех потоках
			vector<TypeProcess*>& _all;
			//блокировка добвления/удаления потоков
			MUTEX& lockAddProcess;

			//Поскольку при работе может вызываться конструктор копии, то данный
			//класс основан на сылочной модели с использованием динамической памяти,
			//контролируемой MemoryManager
			size_t& count_link;

			//Очистка памяти от завершенных потоков
			void clear();

			void lock_process(){
				while (!lockAddProcess.try_lock()){
					std::this_thread::yield();//sleep_for(std::chrono::milliseconds(5));
				}
				//lockAddProcess.lock();
			}
			void unlock_process(){
				lockAddProcess.unlock();
			}

		public:
			ProcessManager(const ProcessManager&mng):
						id(mng.id),
						_th(mng._th),
						_all(mng._all),
						lockAddProcess(mng.lockAddProcess),
						count_link(mng.count_link){
				count_link++;
			}
			ProcessManager(ProcessManager&&mng):
						id(mng.id),
						_th(mng._th),
						_all(mng._all),
						lockAddProcess(mng.lockAddProcess),
						count_link(mng.count_link){
				count_link++;
			}
			~ProcessManager(){
				count_link--;
				if (count_link==0){
					GloBalMemoryManager.erase(id);
					GloBalMemoryManager.erase(_all);
					GloBalMemoryManager.erase(lockAddProcess);
					GloBalMemoryManager.erase(count_link);
					for (auto x=_th.begin();x!=_th.end();x++){
						(*x)->detach();
						(*x)->~thread();
						delete (*x);
					}
					GloBalMemoryManager.erase(_th);
				}
			}
			//Запуск потока.
			//FUNC - запускаемая функция, ничего не принимающая
			//HANDLE - функция, запускаемая после запуска FUNC. На вход должна принимать результат первой
			//name - имя потока для отображения
			//ERROR - функция, обрабатывающая ошибку при работе FUNC или HANDLE
			template <typename FUNC, typename HANDLE,typename ERROR>
			int runing(FUNC manager,HANDLE trigger,const string &name,ERROR er);
			//Запуск потока.
			//FUNC - запускаемая функция, ничего не принимающая
			//HANDLE - функция, запускаемая после запуска FUNC. На вход должна принимать результат первой
			//name - имя потока для отображения
			template <typename FUNC, typename HANDLE>
			inline int runing(FUNC manager,HANDLE trigger,const string &name="empty"){
				return runing(manager,trigger,name,nullfun);
			}
			//Запуск потока.
			//FUNC - запускаемая функция, ничего не принимающая
			//name - имя потока для отображения
			template <typename FUNC>
			int runingWithName(FUNC manager,const string &name);
			template <typename FUNC>
			inline int runing(FUNC manager){
				return runingWithName(manager,"<empty>");
			}
			//Очистка завершенных потоков
			bool operator()(){
				while(true){
					clear();
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
				return true;
			}
			ProcessManager():_th(GloBalMemoryManager.getMem<vector<thread*>>()),
					lockAddProcess(GloBalMemoryManager.getMem<MUTEX>()),
					_all(GloBalMemoryManager.getMem<vector<TypeProcess*>>()),
					count_link(GloBalMemoryManager.getMem<size_t>()),
					id (GloBalMemoryManager.getMem<int>()){
				auto x=*this;
				runingWithName(x,"system");

			}
			//Принудительное завершение потока является очень плохим тоном,
			//поскольку раскрутка стека при этом не происходит и возможны утечки памяти
			#ifdef LINUX
			void kill(int id);
			#endif

			friend ostream& operator<<(ostream& os,const ProcessManager& mng){
				for (auto x=mng._all.begin();x!=mng._all.end();x++){
					auto tmp=(*x);
					os<<(*x)->Info().ID()<<"\t"<<(*x)->Info().countTryRuning()<<"\t"<<
							(*x)->Info().Name()<<"\t\t"
							<<ToString(tmp->status())<<"\n";
				}
				return os;
			}
			void weit(int id);
			void weitAll();
			void weitAll(const vector<int>&);

	};


	//ToDo

	template <typename FUNC>
	int ProcessManager::runingWithName(FUNC manager,const string &name){
		lockAddProcess.lock();
		int is=id++;
		TypeProcess* sys=new TypeProcess(is,name);
		_all.push_back(sys);
		void (*f)(TypeProcess*,FUNC);
		f=easyRun;
		thread* t=new thread(f,sys,manager);
		_th.push_back(t);
		lockAddProcess.unlock();
		return is;

	}
	template <typename FUNC, typename HANDLE,typename ERROR>
	int ProcessManager::runing(FUNC manager,HANDLE trigger,const string &name,ERROR er){
		lockAddProcess.lock();
		int is=id++;
		TypeProcess* sys=new TypeProcess(is,name);
		_all.push_back(sys);
		void (*f)(TypeProcess*,FUNC,HANDLE,ERROR);
		f=run;
		thread* t=new thread(f,sys,manager,trigger,er);
		_th.push_back(t);
		lockAddProcess.unlock();
		return is;
	}
}



#endif /* PROCESSMANAGER_PROCESSMANAGER_H_ */
