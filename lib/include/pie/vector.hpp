#pragma once

#include <pie/allocator.hpp>
#include <pie/cstring.hpp>
#include <pie/initializer_list.hpp>
#include <pie/iterator.hpp>
#include <pie/utility.hpp>

namespace pie
{
	template<class Vector>
	class vector_const_iterator : public pie::iterator<random_access_iterator_tag, typename Vector::value_type>
	{
	public:
		typedef typename Vector::value_type value_type;
		typedef typename Vector::difference_type difference_type;
		typedef typename Vector::const_pointer const_pointer;
		typedef typename Vector::pointer pointer;
		typedef vector_const_iterator<Vector> iterator;

		vector_const_iterator() : m_ptr() { }
		vector_const_iterator(pointer ptr) : m_ptr(ptr) { }

		reference operator*() const
		{
			return *m_ptr;
		}

		const_pointer operator->() const
		{
			return pointer_traits<value_type>::pointer_to(**this);
		}

		iterator & operator++()
		{
			++m_ptr;

			return *this;
		}

		iterator operator++(int)
		{
			iterator tmp = *this;
			++*this;

			return tmp;
		}

		iterator & operator--()
		{
			--m_ptr;

			return *this;
		}

		iterator operator--(int)
		{
			iterator tmp = *this;
			--*this;

			return *tmp;
		}

		iterator & operator+=(difference_type offset)
		{
			m_ptr += offset;

			return *this;
		}

		iterator & operator-=(difference_type offset)
		{
			m_ptr -= offset;

			return *this;
		}

		iterator operator+(difference_type offset) const
		{
			iterator tmp = *this;

			return tmp += offset;
		}

		iterator operator-(difference_type offset) const
		{
			iterator tmp = *this;

			return tmp -= offset;
		}

		difference_type operator-(iterator const & it) const
		{
			return m_ptr - it.m_ptr;
		}

		reference operator[](difference_type offset) const
		{
			return *(*this + offset);
		}

		bool operator==(iterator const & it) const
		{
			return m_ptr == it.m_ptr;
		}

		bool operator!=(iterator const & it) const
		{
			return !(*this == it);
		}

		bool operator<(iterator const & it) const
		{
			return m_ptr < it.m_ptr;
		}

		bool operator>(iterator const & it) const
		{
			return m_ptr > it.m_ptr;
		}

		bool operator<=(iterator const & it) const
		{
			return *this < it;
		}

		bool operator>=(iterator const & it) const
		{
			return *this > it;
		}

		pointer m_ptr;
	};

	template<class Vector>
	class vector_iterator : public vector_const_iterator<Vector>
	{
	public:
		typedef vector_const_iterator<Vector> base;
		typedef vector_const_iterator<typename Vector::value_type> value_type;
		typedef typename Vector::difference_type difference_type;
		typedef typename Vector::const_pointer const_pointer;
		typedef typename Vector::pointer pointer;
		typedef vector_iterator<Vector> iterator;

		vector_iterator() : base() { }
		vector_iterator(pointer ptr) : base(ptr) { }

		reference operator*() const
		{
			return *base::m_ptr;
		}

		const_pointer operator->() const
		{
			return pointer_traits<value_type>::pointer_to(**this);
		}

		iterator & operator++()
		{
			++base::m_ptr;

			return *this;
		}

		iterator operator++(int)
		{
			iterator tmp = *this;
			++*this;

			return tmp;
		}

		iterator & operator--()
		{
			--base::m_ptr;

			return *this;
		}

		iterator operator--(int)
		{
			iterator tmp = *this;
			--*this;

			return *tmp;
		}

		iterator & operator+=(difference_type offset)
		{
			base::m_ptr += offset;

			return *this;
		}

		iterator & operator-=(difference_type offset)
		{
			base::m_ptr -= offset;

			return *this;
		}

		iterator operator+(difference_type offset) const
		{
			iterator tmp = *this;

			return tmp += offset;
		}

		iterator operator-(difference_type offset) const
		{
			iterator tmp = *this;

			return tmp -= offset;
		}

		difference_type operator-(iterator const & it) const
		{
			return base::m_ptr - it.m_ptr;
		}

		reference operator[](difference_type offset) const
		{
			return *(*this + offset);
		}

		bool operator==(iterator const & it) const
		{
			return base::m_ptr == it.m_ptr;
		}

		bool operator!=(iterator const & it) const
		{
			return !(*this == it);
		}

		bool operator<(iterator const & it) const
		{
			return base::m_ptr < it.m_ptr;
		}

		bool operator>(iterator const & it) const
		{
			return base::m_ptr > it.m_ptr;
		}

		bool operator<=(iterator const & it) const
		{
			return *this < it;
		}

		bool operator>=(iterator const & it) const
		{
			return *this > it;
		}
	};

	template<class T, class Allocator = pie::allocator<T>>
	class vector
	{
	public:
		typedef T value_type;
		typedef typename Allocator allocator_type;
		typedef typename Allocator::size_type size_type;
		typedef typename Allocator::difference_type difference_type;
		typedef typename Allocator::reference reference;
		typedef typename Allocator::const_reference const_reference;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;
		typedef vector_iterator<vector<value_type>> iterator;
		typedef vector_const_iterator<vector<value_type>> const_iterator;

		vector() : 
            m_allocator(),
            m_vector(),
            m_size(),
            m_capacity()
        {
        }

		vector(vector const & vec) :
            m_allocator(),
            m_vector(),
            m_size(),
            m_capacity()
		{
            reallocate(vec.size(), false);
            pie::copy(vec.cbegin(), vec.cend(), begin());
		}

        vector(vector && vec) :
            m_allocator(vec.m_allocator),
            m_vector(vec.m_vector),
            m_size(vec.m_size),
            m_capacity(vec.m_capacity)
        {
            vec.m_vector = nullptr;
            vec.m_size = 0;
            m_capacity = 0;
        }

		vector(size_type count) :
            m_allocator(),
            m_vector(),
            m_size(),
            m_capacity()
		{
			reallocate(count, false);
            if (m_vector != nullptr)
                m_size = count;
		}

        virtual ~vector() { }

        // --- Element access ---

        pointer data()
        {
        	return m_vector;
        }

        const_pointer data() const
        {
        	return m_vector;
        }

		// --- Iterators ---        
        iterator begin()
        {
        	return iterator(m_vector);
        }

        const_iterator begin() const
        {
        	return const_iterator(m_vector);
        }

        const_iterator cbegin() const
        {
        	return begin();
        }

        iterator end()
        {
        	return iterator(m_vector + m_size);
        }

        const_iterator end() const
        {
        	return const_iterator(m_vector + m_size);
        }

        const_iterator cend() const
        {
        	return end();
        }

        // --- Capacity
        bool empty() const
        {
        	return begin() == end();
        }

        size_type size() const
        {
            return end() - begin();
        }
        
        size_type max_size() const
        {
        	return m_allocator.max_size();
        }

        size_type reserve(size_type new_cap)
        {
            if (m_capacity < new_cap)
                m_capacity = new_cap;
        }

        // --- Modifiers
        void clear()
        {
        	reallocate(0);
        }

        void assign(size_type count, const value_type & value)
        {
            reallocate(count, false);
        }
        
        template< class InputIt >
        void assign(InputIt first, InputIt last)
        {
        }

        void assign(std::initializer_list<T> ilist)
        {
        }

        //iterator insert(const_iterator pos, value_type const & value)
        //{
        //	iterator it = &m_vector[pos - m_vector];

        //	if (m_count + 1 > m_size)
        //	{
        //		reallocate(m_size + 1);
        //	}

        //	memmove((it + 1).m_ptr, it.m_ptr, (m_count - (it - m_vector)) * sizeof(value_type));
        //	*it = value;
        //	++m_count;

        //	return it;
        //}

        //iterator insert(const_iterator pos, value_type && value)
        //{
        //	iterator it = &m_vector[pos - m_vector];

        //	if (m_count + 1 > m_size)
        //	{
        //		reallocate(m_size + 1);
        //	}

        //	memmove((it + 1).m_ptr, it.m_ptr, (m_count - (it - m_vector)) * sizeof(value_type));
        //	*it = pie::move(value);
        //	++m_count;

        //	return it;
        //}

        //iterator insert(const_iterator pos, size_type count, value_type const & value)
        //{

        //}

        //template<class InputIterator>
        //iterator insert(const_iterator pos, InputIterator first, InputIterator last)
        //{

        //}

        //iterator insert(const_iterator pos, std::initializer_list<value_type> init_list)
        //{

        //}

        void push_back(value_type const & value)
        {
            internal_insert(end(), value);
        }

        void push_back(value_type && value)
        {
            internal_insert(end(), pie::move(value));
        }

        template<class... Args>
        void emplace_back(Args &&... args)
        {
            internal_insert(end(), pie::forward<Args>(args)...);
        }

        template<class... Args>
        void emplace_front(Args &&... args)
        {
            internal_insert(begin(), pie::forward<Args>(args)...);
        }

        void resize(size_type count)
        {
        	cleanup();
        	reallocate(count, false);
        }

        void resize(size_type count, value_type value)
        {
            cleanup();
            reallocate(count);

            if (m_vector != nullptr)
            {
                for (auto & it = begin(); it != end(); ++it)
                {
                    *it = value;
                }
            }
        }

        void resize(size_type count, value_type const & value)
        {
        	cleanup();
        	reallocate(count, false);

        	if (m_vector != nullptr)
        	{
        		for (auto & it = begin(); it != end(); ++i)
        		{
                    m_allocator.construct(it, value);
        		}
        	}
        }

	protected:
	private:
		allocator_type m_allocator;
		pointer m_vector;
        size_type m_size;
        size_type m_capacity;
		size_type m_reserve_multiplier = 2;

		void cleanup()
		{
			if (m_vector != nullptr)
			{
				m_allocator.deallocate(m_vector);
				m_vector = nullptr;
                m_size = 0;
                m_capacity = 0;
			}
		}

		void reallocate(size_type count, bool copy)
		{
            // If the count + current size exceeds our capacity then reallocate
			if (count + m_size > m_capacity)
			{
                size_type new_capacity = m_capacity == 0 || count <= 1? m_reserve_multiplier : m_capacity * m_reserve_multiplier;
				value_type * new_vector = m_allocator.allocate(new_capacity + 1);
                // Was allocation successful?
				if (new_vector != nullptr)
				{
                    m_capacity = new_capacity;
                    // Copy over the existing elements if specified
                    if (copy == true)
                    {
                        // Default construct all the elements. This MUST be done.
                        for (auto it = new_vector; it != new_vector + m_size; ++it)
                        {
                            m_allocator.construct(pie::pointer_traits<pointer>::pointer_to(*it), pie::forward<value_type>(m_vector[it-new_vector]) );
                        }

                        // The elements should now be in a default state. Copy assign the existing elements 
                        /*pie::copy(begin(), end(), new_vector);*/
                    }

                    m_allocator.deallocate(m_vector);
                    m_vector = new_vector;
				}
			}            
		}

        template<class...Args>
        iterator internal_insert(iterator pos, Args &&... args)
        {
            size_type distance = pie::distance(begin(), pos);
            // Is this an insert into an empty vector?
            if (distance == 0)
            {
                // Yes, allocate an element and update pos
                reallocate(1, false);
                pos = begin();
                m_allocator.construct(pos.m_ptr, pie::forward<Args>(args)...);
                ++m_size;

                return pos;
            }

            if (size() < distance)
                return iterator();

            // Reallocate if needed and update pos
            reallocate(m_size + 1, true);
            pos = begin() + distance;
            // Construct the object/POD
            m_allocator.construct(pos.m_ptr, pie::forward<Args>(args)...);
            ++m_size;
            // Yes, shift the range [pos, end) by one to the right

            return begin() + distance;
            
        }
	};
}
