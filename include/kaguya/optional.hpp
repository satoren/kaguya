// Copyright satoren
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "kaguya/config.hpp"

namespace kaguya
{

	struct bad_optional_access :std::exception {};
	struct nullopt_t {};
	template<class T>class optional
	{
		typedef void (optional::*bool_type)() const;
		void this_type_does_not_support_comparisons() const {}
	public:
		optional() : value_(0) {};
		optional(nullopt_t) : value_(0) {};
		optional(const optional& other) : value_(0)
		{
			if (other)
			{
				value_ = new(storage_) T(other.value());
			}
		}
		optional(const T& value)
		{
			value_ = new(storage_) T(value);
		}

		~optional() {
			destruct();
		}
		optional& operator=(nullopt_t) { destruct(); }
		optional& operator=(const optional& other)
		{
			if (other)
			{
				*this = other.value();
			}
			else
			{
				destruct();
			}
			return *this;
		}
		optional& operator=(const T& value)
		{
			if (value_)
			{
				*value_ = value;
			}
			else
			{
				value_ = new(storage_) T(value);
			}
			return *this;
		}

#if KAGUYA_USE_CPP11
		optional(optional&& other)
		{
			value_ = new(storage_) T(std::move(other.value()));
		}
		optional(T&& value)
		{
			value_ = new(storage_) T(std::move(value));
		}
		optional& operator=(optional&& other)
		{
			if (other)
			{
				*this = std::move(other.value());
			}
			else
			{
				destruct();
			}
			return *this;
		}
		optional& operator=(T&& value)
		{
			if (value_)
			{
				*value_ = std::move(value);
			}
			else
			{
				value_ = new(storage_) T(std::move(value));
			}
			return *this;
		}
#endif

		operator bool_type() const
		{
			return value_ != 0 ? &optional::this_type_does_not_support_comparisons : 0;
		}
		T& value()
		{
			if (value_) { return *value_; }
			throw bad_optional_access();
		}
		const T & value() const
		{
			if (value_) { return *value_; }
			throw bad_optional_access();
		}

		const T* operator->() const { if (value_) { return value_; } throw bad_optional_access(); }
		T* operator->() { if (value_) { return value_; }throw bad_optional_access(); }
		const T& operator*() const { return value(); }
		T& operator*() { return value(); }
	private:
		void destruct()
		{
			if (value_)
			{
				value_->~T(); value_ = 0;
			}
		}
#define KAGUYA_USE_CPP11_ALIGNMENT 0
#if KAGUYA_USE_CPP11_ALIGNMENT
		alignas(alignof(T)) char storage_[sizeof(T)];
#else
#if defined(_MSC_VER)
		__declspec(align(__alignof(T))) char storage_[sizeof(T)];
#elif defined(__GNUC__) || defined(__clang__)
		char storage_[sizeof(T)] __attribute__((aligned(__alignof__(T))));
#else
		char storage_[sizeof(T)] __attribute__((aligned(__alignof__(T))));
#endif
#endif
		T* value_;
	};

	template<class T>class optional<T&>
	{
		typedef void (optional::*bool_type)() const;
		void this_type_does_not_support_comparisons() const {}
	public:
		optional() : value_(0) {};
		optional(nullopt_t) : value_(0) {};

		optional(const optional& other) :value_(other.value_) {	}
		optional(T& value) :value_(&value) {	}

		~optional() {
		}
		optional& operator=(nullopt_t) { value_ = 0; }
		optional& operator=(optional& other)
		{
			value_ = other.value_;
		}
		optional& operator=(T& value)
		{
			value_ = &value;
		}
		operator bool_type() const
		{
			return value_ != 0 ? &optional::this_type_does_not_support_comparisons : 0;
		}
		T& value()
		{
			if (value_) { return *value_; }
			throw bad_optional_access();
		}
		const T & value() const
		{
			if (value_) { return *value_; }
			throw bad_optional_access();
		}

		const T* operator->() const { return value_; }
		T* operator->() { return value_; }
		const T& operator*() const { return value(); }
		T& operator*() { return value(); }
	private:
		T* value_;
	};

}