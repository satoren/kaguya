#pragma once

#include <vector>
#include <map>
#include <cassert>
#include "kaguya/config.hpp"
#include "kaguya/lua_ref.hpp"


namespace kaguya
{
	class State;

	//! Reference to Lua userdata
	class  LuaUserData :public LuaRef
	{
		void typecheck()
		{
			if (type() != TYPE_USERDATA)
			{
				except::typeMismatchError(state_, "not user data");
				LuaRef::unref();
			}
		}
		//hide other type functions
		using LuaRef::setField;
		using LuaRef::setFunctionEnv;
		using LuaRef::getFunctionEnv;
		using LuaRef::operator();
		using LuaRef::threadStatus;
		using LuaRef::isThreadDead;
		using LuaRef::costatus;
	public:
		KAGUYA_LUA_REF_EXTENDS_DEFAULT_DEFINE(LuaUserData);
		KAGUYA_LUA_REF_EXTENDS_MOVE_DEFINE(LuaUserData);

		using LuaRef::getField;
		using LuaRef::keys;
		using LuaRef::values;
		using LuaRef::map;
		/**
		* @name operator[]
		* @brief value = table[key];
		* @param key key of table
		* @return reference of field value
		*/
		//@{
		LuaRef operator[](const LuaRef& key)const
		{
			return LuaRef::operator[](key);
		}
		LuaRef operator[](const char* key)const
		{
			return LuaRef::operator[](key);
		}
		LuaRef operator[](const std::string& key)const
		{
			return LuaRef::operator[](key);
		}
		LuaRef operator[](int index)const
		{
			return LuaRef::operator[](index);
		}
		//@}

		using LuaRef::foreach_table;
		using LuaRef::operator->*;
		using LuaRef::getMetatable;
		using LuaRef::setMetatable;
	};

	//! Reference to Lua table
	class LuaTable :public LuaRef
	{
		void typecheck()
		{
			if (type() != TYPE_TABLE)
			{
				except::typeMismatchError(state_, "not table");
				LuaRef::unref();
			}
		}

		//hide other type functions
		using LuaRef::setFunctionEnv;
		using LuaRef::getFunctionEnv;
		using LuaRef::operator();
		using LuaRef::threadStatus;
		using LuaRef::isThreadDead;
		using LuaRef::costatus;
	public:
		KAGUYA_LUA_REF_EXTENDS_DEFAULT_DEFINE(LuaTable);
		KAGUYA_LUA_REF_EXTENDS_MOVE_DEFINE(LuaTable);

		LuaTable(lua_State* state) :LuaRef(state, NewTable())
		{
		}
		LuaTable(lua_State* state, const NewTable& newtable) :LuaRef(state, newtable)
		{
		}

		using LuaRef::getField;
		using LuaRef::setField;
		using LuaRef::keys;
		using LuaRef::values;
		using LuaRef::map;
		using LuaRef::operator[];
		using LuaRef::foreach_table;
		using LuaRef::operator->*;
		using LuaRef::getMetatable;
		using LuaRef::setMetatable;
	};
	class TableKeyReference :public LuaRef
	{
	public:
		friend class LuaRef;
		friend class State;

		//! this is not copy.same assign from LuaRef.
		TableKeyReference& operator=(const TableKeyReference& src)
		{
			parent_.setField(key_, src);
			static_cast<LuaRef&>(*this) = src;
			return *this;
		}

		//! assign from LuaRef
		TableKeyReference& operator=(const LuaRef& src)
		{
			parent_.setField(key_, src);
			static_cast<LuaRef&>(*this) = src;
			return *this;
		}

		//! assign from T
		template<typename T>
		TableKeyReference& operator=(T src)
		{
			parent_.setField(key_, src);
			static_cast<LuaRef&>(*this) = parent_.getField(key_);
			return *this;
		}

		//! register class metatable to lua and set to table
		template<typename T, typename P>
		void setClass(const ClassMetatable<T, P>& reg, bool auto_reg_shared_ptr = true)
		{
			set_class(reg);
		}

		//! set function 
		template<typename T>
		void setFunction(T f)
		{
			parent_.setField(key_, FunctorType(f));
		}

		TableKeyReference(const TableKeyReference& src) :LuaRef(src), parent_(src.parent_), key_(src.key_) {}

		bool operator==(const TableKeyReference& other)const
		{
			return static_cast<const LuaRef&>(*this) == static_cast<const LuaRef&>(other);
		}
		bool operator<(const TableKeyReference& other)const
		{
			return static_cast<const LuaRef&>(*this) < static_cast<const LuaRef&>(other);
		}
		bool operator<=(const TableKeyReference& other)const
		{
			return static_cast<const LuaRef&>(*this) <= static_cast<const LuaRef&>(other);
		}
		bool operator>=(const TableKeyReference& other)const
		{
			return other <= *this;
		}
		bool operator>(const TableKeyReference& other)const
		{
			return other < *this;
		}
		bool operator!=(const TableKeyReference& other)const
		{
			return !(other == *this);
		}

	private:
		template<typename T, typename P>
		void set_class(const ClassMetatable<T, P>& reg)
		{
			LuaRef table(state_, NewTable());
			table.setMetatable(reg.registerClass(state_));
			*this = table;
		}

		TableKeyReference(LuaTable parent, LuaRef key) :LuaRef(parent.getField(key)), parent_(parent), key_(key) {}


		void swap(TableKeyReference& other)throw()
		{
			std::swap(static_cast<LuaRef&>(*this), static_cast<LuaRef&>(other));
			std::swap(parent_, other.parent_);
			std::swap(key_, other.key_);
		}
#if KAGUYA_USE_RVALUE_REFERENCE
		TableKeyReference(TableKeyReference&& src)throw() :LuaRef(), parent_(), key_()
		{
			swap(src);
		}
#endif

		LuaTable parent_;
		LuaRef key_;
	};

	inline bool LuaRef::setFunctionEnv(const LuaTable& env)
	{
		util::ScopedSavedStack save(state_);
		if (type() != TYPE_FUNCTION)
		{
			except::typeMismatchError(state_, "this is not function" + typeName());
			return false;
		}
		push();
		env.push();
#if LUA_VERSION_NUM >= 502
		lua_setupvalue(state_, -2, 1);
#else
		lua_setfenv(state_, -2);
#endif
		return true;
	}
	inline bool LuaRef::setFunctionEnv(NewTable env)
	{
		return setFunctionEnv(LuaTable(state_));
	}

	inline LuaTable LuaRef::getFunctionEnv()
	{
		util::ScopedSavedStack save(state_);
		if (type() != TYPE_FUNCTION)
		{
			except::typeMismatchError(state_, "this is not function" + typeName());
			return LuaRef(state_);
		}
		push();
		lua_getupvalue(state_, -1, 1);
		return LuaTable(state_, StackTop());
	}

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

	inline bool LuaRef::setMetatable(const LuaTable& table)
	{
		if (ref_ == LUA_REFNIL)
		{
			except::typeMismatchError(state_, "is nil");
			return false;
		}
		util::ScopedSavedStack save(state_);
		push();
		int t = lua_type(state_, -1);
		if (t != LUA_TTABLE && t != LUA_TUSERDATA)
		{
			except::typeMismatchError(state_, typeName() + "is not table");
			return false;
		}
		table.push();
		return lua_setmetatable(state_, -2) != 0;
	}

	inline LuaTable LuaRef::getMetatable()const
	{
		if (ref_ == LUA_REFNIL)
		{
			except::typeMismatchError(state_, "is nil");
			return LuaRef(state_);
		}
		util::ScopedSavedStack save(state_);
		push();
		int t = lua_type(state_, -1);
		if (t != LUA_TTABLE && t != LUA_TUSERDATA)
		{
			except::typeMismatchError(state_, typeName() + "is not table");
			return LuaRef(state_);
		}
		lua_getmetatable(state_, -1);
		return LuaRef(state_, StackTop(), NoMainCheck());
	}



	namespace traits
	{
		template<>
		struct arg_get_type<const LuaUserData& > {
			typedef LuaUserData type;
		};
		template< >	struct is_push_specialized<LuaUserData> : integral_constant<bool, true> {};

		template<>
		struct arg_get_type<const LuaTable& > {
			typedef LuaTable type;
		};

		template< >	struct is_push_specialized<LuaTable> : integral_constant<bool, true> {};
		template< >	struct is_push_specialized<TableKeyReference> : integral_constant<bool, true> {};
	}
	namespace types
	{
		template<>
		inline bool strictCheckType(lua_State* l, int index, typetag<LuaUserData>)
		{
			return lua_type(l, index) == LUA_TUSERDATA;
		}
		template<>
		inline bool checkType(lua_State* l, int index, typetag<LuaUserData>)
		{
			return lua_type(l, index) == LUA_TUSERDATA || lua_isnil(l, index);
		}
		template<>
		inline LuaUserData get(lua_State* l, int index, typetag<LuaUserData> tag)
		{
			lua_pushvalue(l, index);
			return LuaUserData(l, StackTop());
		}
		template<>
		inline int push(lua_State* l, const LuaUserData& ref)
		{
			ref.push(l);
			return 1;
		}
		template<>
		inline bool strictCheckType(lua_State* l, int index, typetag<LuaTable>)
		{
			return lua_istable(l, index);
		}
		template<>
		inline bool checkType(lua_State* l, int index, typetag<LuaTable>)
		{
			return lua_istable(l, index) || lua_isnil(l, index);
		}
		template<>
		inline LuaTable get(lua_State* l, int index, typetag<LuaTable> tag)
		{
			lua_pushvalue(l, index);
			return LuaTable(l, StackTop());
		}
		template<>
		inline int push(lua_State* l, const LuaTable& ref)
		{
			ref.push(l);
			return 1;
		}
		template<>
		inline int push(lua_State* l, const TableKeyReference& ref)
		{
			ref.push(l);
			return 1;
		}


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
			for (typename std::map<K, V>::const_iterator it = ref.begin(); it != ref.end(); ++it)
			{
				table[it->first] = it->second;
			}
			table.push(l);
			return 1;
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
