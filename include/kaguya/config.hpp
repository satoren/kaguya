#pragma once

#include<string>
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#ifndef KAGUYA_USE_BOOST
#if __cplusplus >= 201103L
#define KAGUYA_USE_BOOST 0
#else
#define KAGUYA_USE_BOOST 1
#endif
#endif



#if KAGUYA_USE_BOOST
#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#if BOOST_VERSION >= 104800
#include <boost/move/move.hpp>
#endif
#else
#include <functional>
#include <tuple>
#include <memory>
#include <utility>
#include <mutex>
#endif


#ifndef KAGUYA_ERROR_NO_THROW
#define KAGUYA_ERROR_NO_THROW 1
#endif

#ifndef KAGUYA_USE_RVALUE_REFERENCE
#if defined(__cpp_rvalue_references ) || defined(_HAS_RVALUE_REFERENCES)
#define KAGUYA_USE_RVALUE_REFERENCE 1
#else
#define KAGUYA_USE_RVALUE_REFERENCE 0
#endif
#endif

#ifndef KAGUYA_USE_DECLTYPE
#if defined(__cpp_decltype) || __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1600)
#define KAGUYA_USE_DECLTYPE 1
#else
#define KAGUYA_USE_DECLTYPE 0
#endif
#endif



namespace kaguya
{
	namespace standard
	{
#if KAGUYA_USE_BOOST
		using boost::function;
		using boost::tuple;
		using boost::shared_ptr;
		using boost::get;
		using boost::mutex;
		using boost::lock_guard;
#if BOOST_VERSION >= 104800
		using boost::forward;
#else
		template<typename T>T forward(T v) { return v; }
#endif

		std::string to_string(int v)
		{
			char buffer[128] = { 0 };
			snprintf(buffer, sizeof(buffer), "%d", v);
			return buffer;
		}
#else
		using std::function;
		using std::tuple;
		using std::shared_ptr;
		using std::to_string;
		using std::get;
		using std::forward;
		using std::mutex;
		using std::lock_guard;
#endif
	}

}
