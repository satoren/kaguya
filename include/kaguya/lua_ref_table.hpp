#pragma once

#include <vector>
#include <map>
#include <cassert>
#include "kaguya/config.hpp"
#include "kaguya/lua_ref.hpp"


namespace kaguya
{
	class table_key_reference :public LuaRef
	{
	public:
		table_key_reference(LuaRef parent, LuaRef key) :LuaRef(parent.getField(key)), parent_(parent), key_(key) {}
		table_key_reference(const table_key_reference& src) :LuaRef(src), parent_(src.parent_), key_(src.key_) {}
		table_key_reference& operator=(const table_key_reference& src)
		{
			static_cast<LuaRef&>(*this) = src;
			parent_ = src.parent_;
			key_ = src.key_;
			return *this;
		}

#if KAGUYA_USE_RVALUE_REFERENCE
		table_key_reference(table_key_reference&& src)throw() :LuaRef(), parent_(), key_()
		{
			swap(src);
		}
		table_key_reference& operator=(table_key_reference&& src)throw()
		{
			swap(src);
			return *this;
		}
#endif
		void swap(table_key_reference& other)throw()
		{
			std::swap(static_cast<LuaRef&>(*this), static_cast<LuaRef&>(other));
			std::swap(parent_, other.parent_);
			std::swap(key_, other.key_);
		}

		table_key_reference& operator=(const LuaRef& src)
		{
			parent_.setField(key_, src);
			static_cast<LuaRef&>(*this) = src;
			return *this;
		}
		template<typename T>
		table_key_reference& operator=(T src)
		{
			parent_.setField(key_, src);
			static_cast<LuaRef&>(*this) = parent_.getField(key_);
			return *this;
		}

		template<typename T>
		void setClass(const ClassMetatable<T>& reg, bool auto_reg_shared_ptr = true)
		{
			set_meta_table(reg);

			static_cast<LuaRef&>(*this) = parent_.getField(key_);

			(*this)["ptr_wrapper"].set_meta_table(Metatable(types::metatable_name<types::meta_pointer_wrapper<T> >(), reg.name()));
			if (auto_reg_shared_ptr)
			{
				(*this)["shared_ptr"].set_meta_table(ClassMetatable<standard::shared_ptr<T> >(reg.name()));
			}
		}
		void setMetatable(const Metatable& reg)
		{
			set_meta_table(reg);
		}

		template<typename T>
		void setFunction(T f)
		{
			parent_.setField(key_, nativefunction::functor_type(nativefunction::create(f)));
		}
	private:

		void set_meta_table(const Metatable& reg)
		{
			utils::ScopedSavedStack save(state_);
			parent_.push();
			key_.push();
			reg.registerTable(state_);

			lua_settable(state_, -3);
		}

		LuaRef parent_;
		LuaRef key_;
	};


	inline table_key_reference LuaRef::operator[](const LuaRef& key)
	{
		return table_key_reference(*this, key);
	}
	inline table_key_reference LuaRef::operator[](const char* str)
	{
		return table_key_reference(*this, LuaRef(state_, str));
	}
	inline table_key_reference LuaRef::operator[](const std::string& str)
	{
		return table_key_reference(*this, LuaRef(state_, str));
	}
	inline table_key_reference LuaRef::operator[](int index)
	{
		return table_key_reference(*this, LuaRef(state_, index));
	}
}