#pragma once

#include <pie/allocator.hpp>
#include <pie/iterator.hpp>
#include <pie/memory.hpp>

namespace pie
{
	template<class Deque>
	class deque_const_iterator : 
		public pie::iterator<random_access_iterator_tag, typename Deque::value_type>
	{
	public:
		typedef typename Deque::value_type value_type;
		typedef typename Deque::reference reference;
		typedef typename Deque::const_reference const_reference;
		typedef typename Deque::size_type size_type;
		typedef typename Deque::difference_type difference_type;
		typedef typename Deque::pointer pointer;
		typedef typename Deque::const_pointer const_pointer;

		deque_const_iterator() : 
			m_deque(),
			m_offset()
		{
		}

		deque_const_iterator(size_type offset, Deque * deque_ptr) :
			m_offset(offset),
			m_deque(deque_ptr)
		{
		}

		reference operator*() const
		{
			
		}
	protected:
	private:
		size_type m_offset;
		Deque * m_deque;
	};

	namespace detail
	{
		static bool deque_block_size = 32;
		template<class Type, class Allocator>
		class deque_base
		{
		public:
			typedef Type value_type;
			typedef Allocator allocator_type;
			typedef typename allocator_type::size_type size_type;
			typedef typename allocator_type::difference_type difference_type;
			typedef typename value_type & reference;
			typedef typename value_type const & const_reference;
			typedef typename allocator_type::pointer pointer;
			typedef typename allocator_type::const_pointer const_pointer;

			deque_base() :
				m_block_array(),
				m_block_count()
			{
			}
		protected:
			value_type ** m_block_array;
			size_type m_block_count;
			
			size_type get_block(size_type offset) const
			{
				return (offset / 256) % (m_block_count - 1);
			}
		private:
		};
	}

	template<class Type, class Allocator = pie::allocator<Type>>
	class deque : deque_base<Type, Allocator>
	{
	public:

	protected:
	private:
	};
}
