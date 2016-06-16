// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once
#include <memory>

#include "kaguya/config.hpp"
#include "kaguya/traits.hpp"

namespace kaguya
{
	class AnyLuaData
	{
	public:
		int pushToLua(lua_State* state)const
		{
			return empty() ? 0 : holder_->pushToLua(state);
		}

		AnyLuaData() : holder_() { }

		template < typename DataType >
		AnyLuaData(const DataType& v)
			: holder_(new DataHolder<DataType>(v)) { }

		AnyLuaData(const AnyLuaData& other)
			: holder_(other.holder_.get() ? other.holder_->clone() : 0) { }

		AnyLuaData & operator = (AnyLuaData const & rhs)
		{
			if (this != &rhs)
			{
				holder_.reset(rhs.holder_->clone());
			}
			return *this;
		}
#if KAGUYA_USE_CPP11
		AnyLuaData(AnyLuaData&& rhs) = default;
		AnyLuaData & operator = (AnyLuaData&& rhs) = default;
#endif
		bool empty()const { return !holder_.get(); }
	private:
		struct DataHolderBase
		{
			virtual int pushToLua(lua_State* data)const = 0;
			virtual DataHolderBase * clone(void) = 0;
			virtual ~DataHolderBase() {}
		};
		template < typename Type >
		class DataHolder : public DataHolderBase
		{
		public:
			explicit DataHolder(const Type& v) : data_(v) { }

			virtual DataHolderBase * clone(void)
			{
				return new DataHolder(data_);
			}
			virtual int pushToLua(lua_State* state)const
			{
				return lua_type_traits<Type>::push(state, data_);
			}
		private:
			Type data_;
		};
		//specialize for string literal
		template<int N>	struct DataHolder<const char[N]> :DataHolder<std::string> {
			explicit DataHolder(const char* v) : DataHolder<std::string>(std::string(v, v + N)) {}
		};
		template<int N>	struct DataHolder<char[N]> :DataHolder<std::string> {
			explicit DataHolder(const char* v) : DataHolder<std::string>(std::string(v, v + N)) {}
		};
#if KAGUYA_USE_CPP11
		standard::unique_ptr<DataHolderBase> holder_;
#else
		std::auto_ptr<DataHolderBase> holder_;
#endif
	};


	template<>	struct lua_type_traits<AnyLuaData> {
		static int push(lua_State* l, const AnyLuaData& data)
		{
			return data.pushToLua(l);
		}
	};
}
