/*
 * MultiThreadOut.h
 *
 *  Created on: 7 апр. 2017 г.
 *      Author: diman-pro
 */

#ifndef PROCESSMANAGER_MULTITHREADOUT_H_
#define PROCESSMANAGER_MULTITHREADOUT_H_

#include <mutex>
#include <ostream>
#include <istream>

using std::mutex;
using std::ostream;
using std::istream;


namespace Proces_MNG{

	//Класс для блокировки ввода ввывода в многопоточных программах
	template<typename IN,typename OUT>
	class Multi_Process_Out{
		private:
			IN& _in;
			OUT& _out;
			mutex _lock_read;
			mutex _lock_write;
		public:
			Multi_Process_Out(IN& input,OUT& out):_in(input),_out(out){}

			template <typename T>
			friend Multi_Process_Out& operator<<(Multi_Process_Out& os, const T& t){
				os._lock_write.lock();
				(os._out)<<t;
				os._lock_write.unlock();
				return os;
			}
			//Блокировка вывода, исключительно для одного потока
			inline void lock_write(){
				_lock_write.lock();
			}
			//Разблокировка вывода
			inline void unlock_write(){
				_lock_write.unlock();
			}
			//Вывод
			template<typename T>
			inline Multi_Process_Out& write(const T& out){
				_out<<out;
				return *this;
			}

			//Чтение
			template <typename T>
			friend Multi_Process_Out& operator>>(Multi_Process_Out& os, T& t){
				os._lock_read.lock();
				(os._in)>>t;
				os._lock_read.unlock();
				return os;
			}
			//Блокировка чтения
			inline void lock_read(){
				_lock_read.lock();
			}
			//Разблокировка чтения
			inline void unlock_read(){
				_lock_read.unlock();
			}
			//Чтение
			template <typename T>
			inline Multi_Process_Out& read(T& in){
				_in>>in;
				return *this;
			}


	};
	//Глобальная переменная управления вводом/выводом
	//с клавиатуры и консоля для многих потоков
	extern Multi_Process_Out<istream,ostream> console;
}



#endif /* PROCESSMANAGER_MULTITHREADOUT_H_ */
