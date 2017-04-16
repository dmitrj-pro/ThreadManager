/*
 * ProcessInformation.h
 *
 *  Created on: 6 апр. 2017 г.
 *      Author: diman-pro
 */

#ifndef PROCESSMANAGER_PROCESSINFORMATION_H_
#define PROCESSMANAGER_PROCESSINFORMATION_H_

#include <string>

using std::string;

namespace Proces_MNG{
	//Информация о потоке
	class Process_Information{
		private:
			//ИД потока
			int pid;
			//Кол-во попыток запуска
			size_t count_try_runing=0;
			//наименование потока
			string _name;

		public:
			Process_Information(int id):pid(id),_name("not Found"){}
			Process_Information(int id,string name):pid(id),_name(name){}
			inline int ID(){return pid;}
			inline string Name(){return _name;}
			//Проверка потока на актуальность
			//В ходе работы данная функция не используется
			inline bool isRun(){return pid>0;}
			inline int setID(int id){return pid=id;}
			inline size_t fail(){return ++count_try_runing;}
			inline size_t countTryRuning(){
				return count_try_runing;
			}
	};
}



#endif /* PROCESSMANAGER_PROCESSINFORMATION_H_ */
