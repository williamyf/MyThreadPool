// 单件类模板
#pragma once
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/smart_ptr/make_shared.hpp>

#define MY_DECLARE_SINGLETON(theClass,fname)\
	public:\
		static theClass* fname();

#define MY_DECLARE_SINGLETON_AS_BOOSTPTR(theClass,fname)\
	public:\
		static boost::shared_ptr<theClass> fname();

#define MY_IMPLEMENT_SINGLETON(theClass,fname)\
	static theClass* g_p##theClass##_ins = 0;\
	static boost::mutex g_##theClass##_mut;\
	theClass* theClass::fname() {\
		if (!g_p##theClass##_ins) {\
			boost::mutex::scoped_lock _tlock(g_##theClass##_mut);\
			if (!g_p##theClass##_ins) {\
				static theClass _singleton;\
				g_p##theClass##_ins = &_singleton;\
			}\
		}\
		return g_p##theClass##_ins;\
	}

#define MY_IMPLEMENT_SINGLETON_AS_BOOSTPTR(theClass,fname)\
	static boost::shared_ptr<theClass> g_p##theClass##_ins;\
	static boost::mutex g_##theClass##_mut;\
	boost::shared_ptr<theClass> theClass::fname() {\
		if (!g_p##theClass##_ins) {\
			boost::mutex::scoped_lock _tlock(g_##theClass##_mut);\
			if (!g_p##theClass##_ins) {\
				g_p##theClass##_ins = boost::shared_ptr<theClass>(new theClass());\
			}\
		}\
		return g_p##theClass##_ins;\
	}
