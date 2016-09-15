#pragma once

#include <pie/new.hpp>
#include <pie/stdint.hpp>

namespace pie
{
	template<class T>
	class allocator
	{
	public:
		typedef T value_type;
		typedef T * pointer;
		typedef T const * const_pointer;
		typedef T & reference;
		typedef T const & const_reference;
		typedef pie::size_t size_type;
		typedef ptrdiff_t difference_type;

		template<class U> struct rebind { typedef allocator<U> other; };

		allocator() {};
		allocator(allocator<T> const & other) {};
		~allocator() {};

		template<typename U>
		allocator<T> & operator =(allocator<U> &) { return (*this); };
		allocator<T> & operator =(allocator<T> &) { return (*this); };

		size_type max_size() const { return(static_cast<size_type>(-1) / sizeof(value_type)); };

		pointer address(reference value) const
		{
			return (&value);
		}

		const_pointer address(reference value)
		{
			return (&value);
		}

		pointer allocate(size_type count, void const * hint = nullptr)
		{
			if (count <= max_size())
			{
				return (static_cast<pointer>(::operator new(count * sizeof(value_type))));
			}

			return (nullptr);
		}

		void deallocate(pointer ptr, size_t count = 0)
		{
			::operator delete(ptr);
		}

		//// construct from lvalue 
		//void construct(pointer ptr, value_type const & value)
		//{
		//	/*
		//	This is a placebo new operator. Allocation has already taken place elsewhere.
		//	Its purpose is to call the copy constructor for value_type
		//	*/
		//	::new (static_cast<void*>(ptr)) value_type(value);
		//}

		// construct from multiple arguments
		template<typename U, typename... _Args>
		void construct(U * ptr, _Args && ... args)
		{ 
			/*
				This is a placebo new operator. Allocation has already taken place elsewhere.
				Its purpose is to call the appropriate constructor based on the args
			*/
			::new (static_cast<void*>(ptr)) U(pie::forward<_Args>(args)...);
		};
		
		void destroy(pointer ptr)
		{
			((T*)ptr)->~T();
		}
		
		template<class U>
		void destroy(U * ptr)
		{
			ptr->~U();
		}
	protected:
	private:
	};

	template<typename T>
	bool operator ==(allocator<T> const &, allocator<T> const &)
	{
		return (false);
	}

	template<typename T>
	bool operator !=(allocator<T> const &, allocator<T> const &)
	{
		return (true);
	}
};
