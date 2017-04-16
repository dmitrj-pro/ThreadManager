/*
 * Exec.h
 *
 *  Created on: 11 апр. 2017 г.
 *      Author: diman-pro
 */

#ifndef MASTEREXCECUTE_EXEC_H_
#define MASTEREXCECUTE_EXEC_H_

#include <map>

using std::map;

namespace exec{
	//Класс хранит процедуры под указанными именами и позволяет их запустить
	template<typename Name,typename Value,typename Contaner=map<Name,Value>>
	class Exec{
		private:
			Contaner _data;
		public:
			Exec(){}
			inline Value get(const Name& t){
				return _data[t];
			}
			template<typename T>
			inline T exc(const Name& n){
				return _data[n]();
			}
			inline bool contains(Name n){
				for (auto x=_data.begin();x!=_data.end();x++)
					if (x->first==n)
						return true;
				return false;
			}
			template <typename T,typename... Y>
			inline T exc(Name n, Y... __args){
				if (contains(n))
					return _data[n](__args...);
			}
			template <typename... Y>
			inline void exc(Name n, Y... __args){
				if (contains(n))
					_data[n](__args...);
			}
			inline void push(Name n,Value v){
				_data[n]=v;
			}
			inline size_t size(){
				return _data.size();
			}
	};
	template<typename Name,typename Value>
	Exec<Name,Value> getExec(Name name,Value val){
		Exec<Name,Value> res;
		res.push(name,val);
		return res;
	}

}



#endif /* MASTEREXCECUTE_EXEC_H_ */
