#pragma once

#include <vector>
#include <map>
#include <cassert>
#include "kaguya/config.hpp"
#include "kaguya/lua_ref.hpp"


namespace kaguya
{
	class TableKeyReference :public LuaRef
	{
	public:
		TableKeyReference(LuaRef parent, LuaRef key) :LuaRef(parent.getField(key)), parent_(parent), key_(key) {}
		TableKeyReference(const TableKeyReference& src) :LuaRef(src), parent_(src.parent_), key_(src.key_) {}
		TableKeyReference& operator=(const TableKeyReference& src)
		{
			static_cast<LuaRef&>(*this) = src;
			parent_ = src.parent_;
			key_ = src.key_;
			return *this;
		}

#if KAGUYA_USE_RVALUE_REFERENCE
		TableKeyReference(TableKeyReference&& src)throw() :LuaRef(), parent_(), key_()
		{
			swap(src);
		}
		TableKeyReference& operator=(TableKeyReference&& src)throw()
		{
			swap(src);
			return *this;
		}
#endif
		void swap(TableKeyReference& other)throw()
		{
			std::swap(static_cast<LuaRef&>(*this), static_cast<LuaRef&>(other));
			std::swap(parent_, other.parent_);
			std::swap(key_, other.key_);
		}

		TableKeyReference& operator=(const LuaRef& src)
		{
			parent_.setField(key_, src);
			static_cast<LuaRef&>(*this) = src;
			return *this;
		}
		template<typename T>
		TableKeyReference& operator=(T src)
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

			(*this)["ptr_wrapper"].set_meta_table(Metatable(types::metatableName<types::MetaPointerWrapper<T> >(), reg.name()));
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
			parent_.setField(key_, nativefunction::FunctorType(nativefunction::create(f)));
		}
	private:

		void set_meta_table(const Metatable& reg)
		{
			util::ScopedSavedStack save(state_);
			parent_.push(state_);
			key_.push(state_);
			reg.registerTable(state_);

			lua_settable(state_, -3);
		}

		LuaRef parent_;
		LuaRef key_;
	};


	inline TableKeyReference LuaRef::operator[](const LuaRef& key)
	{
		return TableKeyReference(*this, key);
	}
	inline TableKeyReference LuaRef::operator[](const char* str)
	{
		return TableKeyReference(*this, LuaRef(state_, str));
	}
	inline TableKeyReference LuaRef::operator[](const std::string& str)
	{
		return TableKeyReference(*this, LuaRef(state_, str));
	}
	inline TableKeyReference LuaRef::operator[](int index)
	{
		return TableKeyReference(*this, LuaRef(state_, index));
	}
}