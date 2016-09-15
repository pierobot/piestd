#pragma once

#include <pie/stdint.hpp>

#ifndef USE_STD_INIT_LIST

/*
	Namespace hack... 
	Visual Studio requires initializer_list to be in the std namespace.
*/
namespace std
{
	/* This list can, and will, be used by range-based for loops and other algorithms that operate on iterators. */
	template<typename _Type>
	class initializer_list
	{
		typedef _Type value_type;
		typedef _Type const & const_reference;
		typedef const_reference reference;
		typedef pie::size_t size_type;
		typedef _Type const * const_iterator;
		typedef const_iterator iterator;
	public:
		initializer_list() : first(nullptr), last(nullptr) {};
		initializer_list(_Type const * _first, _Type const * _last) : first(_first), last(_last) {};
		iterator begin() { return (first); };
		iterator end() { return (last); };

		size_type size() { return (static_cast<size_type>(last - first)); };
	private:
		value_type const * first;
		value_type const * last;
	};

	template<typename _Type>
	_Type const * begin(initializer_list<_Type> init_list)
	{
		return (init_list.begin());
	}

	template<typename _Type>
	_Type const * end(initializer_list<_Type> init_list)
	{
		return (init_list.end());
	}
};

/* But I also want it in the pie namespace :) */
namespace pie
{
	template<typename _Type>
	using initializer_list = std::initializer_list<_Type>;
};

#else
#	include <initializer_list>
#endif
