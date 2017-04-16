/*
 * SaveResult.h
 *
 *  Created on: 7 апр. 2017 г.
 *      Author: diman-pro
 */

#ifndef SAVERESULT_SAVERESULT_H_
#define SAVERESULT_SAVERESULT_H_

#include <utility>
#include "../ProcessManager/ProcessManager.h"
#include "../Sys.h"


#define GloBalMemoryManager memory

using std::vector;
using std::mutex;
using Proces_MNG::ProcessManager;


namespace SaveResultThread{
	template<typename R>
	class TmpStruct{
		private:
			vector<R>& result;
			mutex& lock;
		public:
			TmpStruct():result(GloBalMemoryManager.getMem<vector<R>>()),lock(GloBalMemoryManager.getMem<mutex>()){}
			void push_back(const R& x){
				lock.lock();
				result.push_back(x);
				lock.unlock();
			}
			vector<R> Res(){
				return result;
			}
	};

	template<typename T,typename ResType=decltype(std::declval<T>()())>
	class Save{
		private:
			TmpStruct<ResType>& data;
			vector<int> ids;
			int&count_link;

			T func;
			size_t count;
			ProcessManager& mng;
		public:
			~Save(){
				count_link--;
				if (count_link==0){
					Proces_MNG::console<<"distruct";
					GloBalMemoryManager.erase(data);
				}
			}
			Save(T f,size_t threads):
				data(GloBalMemoryManager.getMem<TmpStruct<ResType>>()),
				func(f),
				count(threads),
				count_link(GloBalMemoryManager.getMem<int>()),
				mng(GloBalMemoryManager.getMem<ProcessManager>()){
				count_link=1;
			}
			Save(ProcessManager& mng,T f,size_t threads):
				data(GloBalMemoryManager.getMem<TmpStruct<ResType>>()),
				mng(mng),
				func(f),
				count_link(GloBalMemoryManager.getMem<int>()),
				count(threads){
				count_link=1;
			}
			Save(const Save& s):data(s.data),func(s.func),
					count(s.count),
					mng(s.mng),
					count_link(s.count_link){
				count_link++;
			}
			void operator()(const ResType& r){
				data.push_back(r);
			}
			void operator()(ResType& r){
				data.push_back(r);
			}
			void run(){
				for (int i=0;i<count;i++){
					ids.push_back(mng.runing(func,*this));
				}
			}
			void weit(){
				mng.weitAll(ids);
			}
			vector<ResType> Result(){
				return data.Res();
			}

	};

	template<typename T>
	Save<T> getSave(T func,size_t count){
		return Save<T>(func,count);
	}
	template<typename T>
	Save<T> getSave(ProcessManager& mng, T func,size_t count){
		return Save<T>(mng,func,count);
	}

}


#endif /* SAVERESULT_SAVERESULT_H_ */
