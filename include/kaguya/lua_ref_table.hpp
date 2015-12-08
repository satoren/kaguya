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

			(*this)["ptr_wrapper"].set_meta_table(Metatable(types::metatableName<MetaPointerWrapper<T> >(), reg.name()));
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
	
	
	
	
	namespace types
	{
	
		//vector and map to Lua table 
		template<typename T>
		inline bool strictCheckType(lua_State* l, int index, typetag<std::vector<T> >)
		{
			LuaRef table = get(l, index, typetag<LuaRef>());
			if (table.type() != LuaRef::TYPE_TABLE) { return false; }
			std::map<LuaRef, LuaRef> values = table.map();
			for (std::map<LuaRef, LuaRef>::const_iterator it = values.begin(); it != values.end(); ++it)
			{
				if (!it->first.typeTest<size_t>() || !it->second.typeTest<T>())
				{
					return false;
				}
			}
			return true;
		}
		template<typename T>
		inline bool checkType(lua_State* l, int index, typetag<std::vector<T> >)
		{
			LuaRef table = get(l, index, typetag<LuaRef>());
			if (table.type() != LuaRef::TYPE_TABLE) { return false; }
			std::map<LuaRef, LuaRef> values = table.map();
			for (std::map<LuaRef, LuaRef>::const_iterator it = values.begin(); it != values.end(); ++it)
			{
				if (!it->first.typeTest<size_t>() || !it->second.weakTypeTest<T>())
				{
					return false;
				}
			}
			return true;
		}
		template<typename T>
		inline std::vector<T> get(lua_State* l, int index, typetag<std::vector<T> > tag)
		{
			std::vector<T> result;
			LuaRef table = get(l, index, typetag<LuaRef>());
			std::vector<LuaRef> values = table.values();
			result.reserve(values.size());
			for (std::vector<LuaRef>::iterator it = values.begin(); it != values.end(); ++it)
			{
				result.push_back(it->get<T>());
			}
			return result;
		}
		template<typename T>
		inline int push(lua_State* l, const std::vector<T>& ref)
		{
			LuaRef table(l, NewTable(int(ref.size()), 0));

			int count = 1;//array is 1 origin in Lua
			for (typename std::vector<T>::const_iterator it = ref.begin(); it != ref.end(); ++it)
			{
				table[count++] = *it;
			}
			table.push(l);
			return 1;
		}
		template<typename T>
		inline int push(lua_State* l, std::vector<T>& ref)
		{
			return push(l, const_cast<const std::vector<T>&>(ref));
		}

		//std::map
		template<typename K, typename V>
		inline bool strictCheckType(lua_State* l, int index, typetag<std::map<K, V> >)
		{
			LuaRef table = get(l, index, typetag<LuaRef>());
			if (table.type() != LuaRef::TYPE_TABLE) { return false; }
			std::map<LuaRef, LuaRef> values = table.map();
			for (std::map<LuaRef, LuaRef>::const_iterator it = values.begin(); it != values.end(); ++it)
			{
				if (!it->first.typeTest<K>() || !it->second.typeTest<V>())
				{
					return false;
				}
			}
			return true;
		}
		template<typename K, typename V>
		inline bool checkType(lua_State* l, int index, typetag<std::map<K, V> >)
		{
			LuaRef table = get(l, index, typetag<LuaRef>());
			if (table.type() != LuaRef::TYPE_TABLE) { return false; }
			std::map<LuaRef, LuaRef> values = table.map();
			for (std::map<LuaRef, LuaRef>::const_iterator it = values.begin(); it != values.end(); ++it)
			{
				if (!it->first.typeTest<K>() || !it->second.weakTypeTest<V>())
				{
					return false;
				}
			}
			return true;
		}
		template<typename K, typename V>
		inline std::map<K, V> get(lua_State* l, int index, typetag<std::map<K, V> > tag)
		{
			std::map<K, V> result;
			LuaRef table = get(l, index, typetag<LuaRef>());
			std::map<LuaRef, LuaRef> values = table.map();
			for (std::map<LuaRef, LuaRef>::const_iterator it = values.begin(); it != values.end(); ++it)
			{
				result[it->first.get<K>()] = it->second.get<V>();
			}
			return result;
		}
		template<typename K, typename V>
		inline int push(lua_State* l, const std::map<K, V>& ref)
		{
			LuaRef table(l, NewTable(0, int(ref.size())));
			for (typename std::map<K,V>::const_iterator it = ref.begin(); it != ref.end(); ++it)
			{
				table[it->first] = it->second;
			}
			table.push(l);
			return 1;
		}
		template<typename K, typename V>
		inline int push(lua_State* l, std::map<K, V>& ref)
		{
			return push(l, const_cast<const std::map<K, V>&>(ref));
		}
	}
	namespace traits
	{
		template<class V >
		struct arg_get_type<const std::vector<V>& > {
			typedef std::vector<V> type;
		};
		template<class K, class V >
		struct arg_get_type<const std::map<K, V>& > {
			typedef std::map<K, V> type;
		};
	}
}