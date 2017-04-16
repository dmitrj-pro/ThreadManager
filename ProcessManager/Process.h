/*
 * Process.h
 *
 *  Created on: 6 апр. 2017 г.
 *      Author: diman-pro
 */

#ifndef PROCESSMANAGER_PROCESS_H_
#define PROCESSMANAGER_PROCESS_H_

#include "ProcessInformation.h"
#include <string>
#include "sys.h"
#ifdef LINUX
#include <cxxabi.h>
#endif

using std::string;

namespace Proces_MNG{

	inline void nullfun(){}

	//Статус потока:
	//NIL - системная ошибка
	//NotRunning - не запущен
	//Runing - запущен
	//Fail - Ошибка запуска потока
	//Succes - поток завершен
	enum class ProcessStatus {NIL,NotRunning,Running,Fail,Succes};

	//Преобразование статуса потока
	string ToString(const ProcessStatus& st);

	//Поток
	class Process{
		private:
			//Информация о потоке
			Process_Information info;
			//Статус потока
			ProcessStatus stat;

		public:
			//Максимальное количество попыток запуска одного потока
			size_t MaxTryRunning=20;
			Process (int id,string name):info(Process_Information(id,name)){
				stat=ProcessStatus::NotRunning;
			}
			virtual ~Process(){}
			//Запуск потока.
			//FUNC - запускаемая функция, ничего не принимающая
			//HANDLE - функция, запускаемая после запуска FUNC. На вход должна принимать результат первой
			//ERROR - функция, обрабатывающая ошибку при работе FUNC или HANDLE
			template <typename FUNC, typename HANDLE,typename ERROR>
			void run(FUNC f,HANDLE tr,ERROR er=nullfun){
				#ifdef LINUX
				try{
				#endif
					stat=ProcessStatus::Running;
					while(true){

						try{
							auto res=f();
							tr(res);
							break;
						} catch(...){
							if (info.fail()==MaxTryRunning){
								stat=ProcessStatus::Fail;
								return;
							}
							er();
							continue;
						}
					}
					stat=ProcessStatus::Succes;
				#ifdef LINUX
				}catch (abi::__forced_unwind&)
			    {  // handle pthread_cancel stack unwinding exception
			        throw;
			    }
			    catch (std::exception &ex)
			    {
			        throw ex;
			    }
				#endif
			}
			//Запуск потока.
			//FUNC - запускаемая функция, ничего не принимающая
			//ERROR - функция, обрабатывающая ошибку при работе FUNC
			template <typename FUNC, typename ERROR>
			void run(FUNC f,ERROR er=nullfun){
				#ifdef LINUX
				try{
				#endif
					stat=ProcessStatus::Running;
					while(true){
						try{
							f();
							break;
						}
						catch(...){
							if (info.fail()==MaxTryRunning){
								stat=ProcessStatus::Fail;
								return;
							}
							continue;
						}
							return;
					}
					stat=ProcessStatus::Succes;
				#ifdef LINUX
				}catch (abi::__forced_unwind&)
			    {  // handle pthread_cancel stack unwinding exception
					//pthread_exit(0);
					throw;
			    }
				#endif
			}
			template <typename FUNC>
			void run(FUNC f){
				run(f,nullfun);
			}
			inline virtual ProcessStatus status(){
				return stat;
			}
			inline virtual Process_Information Info(){
				return info;
			}
	};
}




#endif /* PROCESSMANAGER_PROCESS_H_ */
