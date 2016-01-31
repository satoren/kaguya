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
		using LuaRef::resume;
		using LuaRef::call;
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
		using LuaRef::resume;
		using LuaRef::call;
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

	/**
	* This class is the type returned by members of non-const LuaRef(Table) when directly accessing its elements.
	*/
	class TableKeyReference
	{
	public:
		friend class LuaRef;
		friend class State;


		template<typename T>
		TableKeyReference operator[](const T& key)
		{
			return getValue()[key];
		}

		template<typename T>
		operator T()const {
			return getValue().get<T>();
		}

		//! this is not copy.same assign from LuaRef.
		TableKeyReference& operator=(const TableKeyReference& src)
		{
			parent_.setField(key_, src);
			return *this;
		}

		//! assign from LuaRef
		TableKeyReference& operator=(const LuaRef& src)
		{
			parent_.setField(key_, src);
			return *this;
		}

		//! assign from T
		template<typename T>
		TableKeyReference& operator=(T src)
		{
			parent_.setField(key_, standard::forward<T>(src));
			return *this;
		}

		bool isNilref()const {
			return getValue().isNilref();
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


		bool operator==(const TableKeyReference& other)const
		{
			return getValue() == other.getValue();
		}
		bool operator<(const TableKeyReference& other)const
		{
			return getValue() < other.getValue();
		}
		bool operator<=(const TableKeyReference& other)const
		{
			return getValue() <= other.getValue();
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

		LuaRef getValue()const
		{
			return parent_.getField(key_);
		}

		operator LuaRef() const {
			return getValue();
		}
		operator LuaTable() const {
			return getValue();
		}
		operator LuaUserData() const {
			return getValue();
		}
		operator LuaFunction() const {
			return getValue();
		}
		operator LuaThread() const {
			return getValue();
		}

		/**
		* @brief table->*"function_name"() in c++ and table:function_name(); in lua is same
		* @param function_name function_name in table
		*/
		mem_fun_binder operator->*(const char* function_name)
		{
			return getValue()->*(function_name);
		}

		/** call lua function
		* template<class... Args>FunctionResults operator()(Args... args);
		*/

#define KAGUYA_DELEGATE_LUAREF getValue()
#include "kaguya/delegate_to_luaref.inl"
#undef KAGUYA_DELEGATE_LUAREF


#if KAGUYA_USE_CPP11
	private://cant resolve compile error on travis-ci g++ with c++03
		//msg error: no matching function for call to 'forward(const t_02_classreg::ABC&)'
#endif
		///!constructs the reference. Accessible only to kaguya::LuaRef itself 
		TableKeyReference(const TableKeyReference& src) : parent_(src.parent_), key_(src.key_) {}
#if KAGUYA_USE_RVALUE_REFERENCE
		///!constructs the reference. Accessible only to kaguya::LuaRef itself 
		TableKeyReference(TableKeyReference&& src)throw() : parent_(), key_()
		{
			swap(src);
		}
#endif
	private:

		template<typename T, typename P>
		void set_class(const ClassMetatable<T, P>& reg)
		{
			LuaRef table(parent_.state_, NewTable());
			table.setMetatable(reg.registerClass(parent_.state_));
			*this = table;
		}

		///!constructs the reference. Accessible only to kaguya::LuaRef itself 
		TableKeyReference(const LuaTable& parent, const LuaRef& key) : parent_(parent), key_(key) {}


#if KAGUYA_USE_RVALUE_REFERENCE
		TableKeyReference(LuaTable&& parent, LuaRef&& key) : parent_(standard::forward<LuaTable>(parent)), key_(standard::forward<LuaRef>(key)) {}
#endif

		void swap(TableKeyReference& other)throw()
		{
			std::swap(parent_, other.parent_);
			std::swap(key_, other.key_);
		}

		LuaTable parent_;
		LuaRef key_;
	};


	inline std::ostream& operator<<(std::ostream& os, const TableKeyReference& ref)
	{
		ref.getValue().dump(os);
		return os;
	}

	inline const LuaRef& toLuaRef(const LuaUserData& ref)
	{
		return static_cast<const LuaRef&>(ref);
	}
	inline const LuaRef& toLuaRef(const LuaTable& ref)
	{
		return static_cast<const LuaRef&>(ref);
	}
	inline LuaRef toLuaRef(const TableKeyReference& ref)
	{
		return ref.getValue();
	}

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


	template<typename T>
	bool operator==(const TableKeyReference& lhs, const T& rhs)
	{
		return lhs.getValue() == rhs;
	}
	template<typename T>
	bool operator<(const TableKeyReference& lhs, const T& rhs)
	{
		return lhs.getValue() < rhs;
	}
	template<typename T>
	bool operator<=(const TableKeyReference& lhs, const T& rhs)
	{
		return lhs.getValue() <= rhs;
	}
	template<typename T>
	bool operator>=(const TableKeyReference& lhs, const T& rhs)
	{
		return rhs <= lhs;
	}
	template<typename T>
	bool operator>(const TableKeyReference& lhs, const T& rhs)
	{
		return rhs < lhs;
	}
	template<typename T>
	bool operator!=(const TableKeyReference& lhs, const T& rhs)
	{
		return !(rhs == lhs);
	}
	template<typename T>
	bool operator==(const T& lhs, const TableKeyReference& rhs)
	{
		return lhs == rhs.getValue();
	}
	template<typename T>
	bool operator<(const T& lhs, const TableKeyReference& rhs)
	{
		return lhs < rhs.getValue();
	}
	template<typename T>
	bool operator<=(const T& lhs, const TableKeyReference& rhs)
	{
		return lhs <= rhs.getValue();
	}
	template<typename T>
	bool operator>=(const T& lhs, const TableKeyReference& rhs)
	{
		return rhs <= lhs;
	}
	template<typename T>
	bool operator>(const T& lhs, const TableKeyReference& rhs)
	{
		return rhs < lhs;
	}
	template<typename T>
	bool operator!=(const T& lhs, const TableKeyReference& rhs)
	{
		return !(rhs == lhs);
	}

	template<>	struct lua_type_traits<LuaUserData> {
		typedef LuaUserData get_type;
		typedef LuaUserData push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_type(l, index) == LUA_TUSERDATA;
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_type(l, index) == LUA_TUSERDATA || lua_isnil(l, index);
		}
		static LuaUserData get(lua_State* l, int index)
		{
			lua_pushvalue(l, index);
			return LuaUserData(l, StackTop());
		}
		static int push(lua_State* l, const LuaUserData& ref)
		{
			ref.push(l);
			return 1;
		}
	};
	template<>	struct lua_type_traits<const LuaUserData&> :lua_type_traits<LuaUserData> {};


	template<>	struct lua_type_traits<LuaTable> {
		typedef LuaTable get_type;
		typedef LuaTable push_type;

		static bool strictCheckType(lua_State* l, int index)
		{
			return lua_istable(l, index);
		}
		static bool checkType(lua_State* l, int index)
		{
			return lua_istable(l, index) || lua_isnil(l, index);
		}
		static LuaTable get(lua_State* l, int index)
		{
			lua_pushvalue(l, index);
			return LuaTable(l, StackTop());
		}
		static int push(lua_State* l, const LuaTable& ref)
		{
			ref.push(l);
			return 1;
		}
	};
	template<>	struct lua_type_traits<const LuaTable&> :lua_type_traits<LuaTable> {};


	template<>	struct lua_type_traits<TableKeyReference> {
		static int push(lua_State* l, const TableKeyReference& ref)
		{
			ref.getValue().push(l);
			return 1;
		}
	};

#ifndef KAGUYA_NO_STD_VECTOR_TO_TABLE
	template<typename T, typename A>
	struct lua_type_traits<std::vector<T, A> >
	{
		typedef std::vector<T, A> get_type;
		typedef const std::vector<T, A>& push_type;

		static bool checkType(lua_State* l, int index)
		{
			LuaRef table = lua_type_traits<LuaRef>::get(l, index);
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
		static bool strictCheckType(lua_State* l, int index)
		{
			LuaRef table = lua_type_traits<LuaRef>::get(l, index);
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

		static get_type get(lua_State* l, int index)
		{
			get_type result;
			LuaRef table = lua_type_traits<LuaRef>::get(l, index);
			std::vector<LuaRef> values = table.values();
			result.reserve(values.size());
			for (std::vector<LuaRef>::iterator it = values.begin(); it != values.end(); ++it)
			{
				result.push_back(it->get<T>());
			}
			return result;
		}
		static int push(lua_State* l, push_type v)
		{
			LuaRef table(l, NewTable(int(v.size()), 0));

			int count = 1;//array is 1 origin in Lua
			for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it)
			{
				table.setField(count++, *it);
			}
			table.push(l);
			return 1;
		}
	};
#endif

#ifndef KAGUYA_NO_STD_MAP_TO_TABLE
	template<typename K, typename V, typename C, typename A>
	struct lua_type_traits<std::map<K, V, C, A> >
	{
		typedef std::map<K, V, C, A> get_type;
		typedef const std::map<K, V, C, A>& push_type;

		static bool checkType(lua_State* l, int index)
		{
			LuaRef table = lua_type_traits<LuaRef>::get(l, index);
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
		static bool strictCheckType(lua_State* l, int index)
		{
			LuaRef table = lua_type_traits<LuaRef>::get(l, index);
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

		static get_type get(lua_State* l, int index)
		{
			get_type result;
			LuaRef table = lua_type_traits<LuaRef>::get(l, index);
			std::map<LuaRef, LuaRef> values = table.map();
			for (std::map<LuaRef, LuaRef>::const_iterator it = values.begin(); it != values.end(); ++it)
			{
				result[it->first.get<K>()] = it->second.get<V>();
			}
			return result;
		}
		static int push(lua_State* l, push_type v)
		{
			LuaRef table(l, NewTable(0, int(v.size())));
			for (typename std::map<K, V>::const_iterator it = v.begin(); it != v.end(); ++it)
			{
				table.setField(it->first, it->second);
			}
			table.push(l);
			return 1;
		}
	};
#endif
}
