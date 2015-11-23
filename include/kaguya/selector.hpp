#pragma once

#include "kaguya/config.hpp"
#include "kaguya/lua_ref.hpp"
#include "kaguya/metatable.hpp"
#include "kaguya/evaluator.hpp"

namespace kaguya
{
	class Selector
	{
	public:
		Selector(lua_State* state, std::string name, ErrorHandler& error_handler) :state_(state),error_handler_(error_handler), name_(name), key_(state, name)
		{
		}


		template<typename T>
		void setClass(const ClassMetatable<T>& reg, bool auto_reg_shared_ptr = true)
		{
			set_meta_table(reg);

			(*this)["ptr_wrapper"].set_meta_table(Metatable(types::metatable_name<types::meta_pointer_wrapper<T> >(), reg.name()));
			if (auto_reg_shared_ptr)
			{
				(*this)["shared_ptr"].set_meta_table(Metatable(types::metatable_name<standard::shared_ptr<T> >(), reg.name()));
			}
		}

		void setMetatable(const Metatable& reg)
		{
			set_meta_table(reg);
		}

		template<typename T>
		void operator=(T v)
		{
			put(v);
		}
		template<typename T>
		void setFunction(T f)
		{
			put(nativefunction::functor_type(nativefunction::create(f)));
		}

		template<typename T>
		T get()const
		{
			utils::ScopedSavedStack save(state_);
			getTable();
			return types::get(state_, -1, types::type_tag<T>());
		}

		template<typename T>
		operator T()const {
			return get<T>();
		}
		// can not support msvc..
		//template<typename T>
		//operator T&()  {
		//  utils::ScopedSavedStack save(state_);
		//  getTable();
		//  return types::get(state_,-1,types::type_tag<T&>());
		//}

		template<typename T>
		operator T*() const {
			utils::ScopedSavedStack save(state_);
			getTable();
			return types::get(state_, -1, types::type_tag<T*>());
		}

		operator bool()const {
			utils::ScopedSavedStack save(state_);
			getTable();
			return types::get(state_, -1, types::type_tag<bool>());
		}
		operator std::string()const {
			utils::ScopedSavedStack save(state_);
			getTable();
			return types::get(state_, -1, types::type_tag<std::string>());
		}
		operator float()const {
			utils::ScopedSavedStack save(state_);
			getTable();
			return types::get(state_, -1, types::type_tag<float>());
		}
		operator double()const {
			utils::ScopedSavedStack save(state_);
			getTable();
			return types::get(state_, -1, types::type_tag<double>());
		}

		operator unsigned int()const {
			utils::ScopedSavedStack save(state_);
			getTable();
			return types::get(state_, -1, types::type_tag<unsigned int>());
		}
		operator int()const {
			utils::ScopedSavedStack save(state_);
			getTable();
			return types::get(state_, -1, types::type_tag<int>());
		}
		operator int64_t()const {
			utils::ScopedSavedStack save(state_);
			getTable();
			return types::get(state_, -1, types::type_tag<int64_t>());
		}

		bool operator == (bool v)const { return get<bool>() == v; }
		bool operator == (const std::string& v)const { return get<std::string>() == v; }
		bool operator == (const char* v)const { return get<std::string>() == v; }
		bool operator == (float v)const { return get<float>() == v; }
		bool operator == (double v)const { return get<double>() == v; }
		bool operator == (int v)const { return get<int>() == v; }
		bool operator == (int64_t v)const { return get<int64_t>() == v; }

		template<typename T>
		bool operator == (const T* v)const
		{
			return get<T>() == v;
		}
		template<typename T>
		bool operator != (const T& v)const
		{
			return !((*this) == v);
		}


		Selector operator[](const char* str)
		{
			return (*this)[std::string(str)];
		}

		Selector operator[](const std::string& str)
		{
			checkTable();
			std::string name; name.reserve(name_.size() + 1 + str.size());
			name += name_; name += "."; name.insert(name.end(), str.begin(), str.end());

			std::vector<LuaRef> path = path_;
			path.push_back(key_);

			return Selector(state_, name, path, LuaRef(state_, str), error_handler_);
		}
		Selector operator[](int index)
		{
			std::string str = "[" + standard::to_string(index) + "]";
			checkTable();
			std::string name = name_ + str;

			std::vector<LuaRef> path = path_;
			path.push_back(key_);

			return Selector(state_, name, path, LuaRef(state_, index), error_handler_);
		}

		//need RVO support compiler
#include "kaguya/gen/evaluator_function.inl"

		std::string typeName()const
		{
			return lua_typename(state_, type());
		}
	private:

		void set_meta_table(const Metatable& reg)
		{
			utils::ScopedSavedStack save(state_);
			setupPath();
			key_.push();
			reg.registerTable(state_);

			lua_settable(state_, -3);
		}

		Selector(lua_State* state, std::string name, std::vector<LuaRef> path, LuaRef key,ErrorHandler& error_handler) :state_(state), error_handler_(error_handler), name_(name), path_(path), key_(key)
		{
		}

		template<typename T>
		void put(T v)
		{
			utils::ScopedSavedStack save(state_);
			setupPath();
			key_.push();
			types::push(state_, standard::forward<T>(v));
			lua_settable(state_, -3);
			lua_pop(state_, 1);
		}
		void setupPath()const
		{
			lua_pushglobaltable(state_);
			for (std::vector<LuaRef>::const_iterator i = path_.begin(); i != path_.end(); ++i)
			{
				getTable(*i);
			}
		}

		void getTable(const LuaRef& ref)const
		{
			ref.push();
			lua_gettable(state_, -2);
			lua_remove(state_, lua_absindex(state_, -2));
		}
		void getTable()const
		{
			setupPath();
			getTable(key_);
		}

		//return lua type
		int type()const
		{
			utils::ScopedSavedStack save(state_);
			getTable();
			return lua_type(state_, -1);
		}

		void checkTable() {
			utils::ScopedSavedStack save(state_);
			if (type() != LUA_TTABLE) {
				put(types::newtable_tag());
			};
		}

		lua_State *state_;
		ErrorHandler& error_handler_;
		std::string name_;
		LuaRef key_;
		std::vector<LuaRef> path_;
	};
}
