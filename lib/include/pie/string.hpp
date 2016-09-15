#pragma once

#ifdef PIE_USE_STD
#   include <string>
    
namespace pie = std;
#else
#   include <pie/allocator.hpp>
#   include <pie/algorithm.hpp>
#   include <pie/iterator.hpp>
#   include <pie/initializer_list.hpp>
#   include <pie/memory.hpp>
#   include <pie/utility.hpp>

namespace pie
{
    template<class CharType, class IntType>
    struct char_traits
    {
        typedef CharType char_type;
        typedef IntType int_type;
        typedef pie::size_t pos_type;
        typedef pie::size_t off_type;

        static void assign(char_type & dest, char_type const & src)
        {
            dest = src;
        }

        static char_type * assign(char_type * dest, pie::size_t count, char_type src)
        {
            for (pie::size_t i = 0; i < count; i++)
                assign(dest[i], src);

            return dest;
        }

        static bool eq(char_type a, char_type b)
        {
            return a == b;
        }

        static bool lt(char_type a, char_type b)
        {
            return a < b;
        }
        
        static char_type * copy(char_type * dest, char_type const * src, pie::size_t count)
        {
            for (pie::size_t i = 0; i < count; i++)
                dest[i] = src[i];

            return dest;
        }

        static char_type * move(char_type * dest, char_type const * src, pie::size_t count)
        {
            return static_cast<char_type *>(pie::memmove(dest, src, count));
        }

        static int compare(char_type const * a, char_type const * b, pie::size_t count)
        {
            for (pie::size_t i = 0; i < count; ++i)
            {
                if (lt(a[i], b[i]) == true)
                    return -1;
                else if (lt(b[i], a[i]) == true)
                    return 1;
            }

            return 0;
        }

        static pie::size_t length(char_type const * c)
        {
            pie::size_t len = 0;
            while (eq(c[len], char_type()) == false)
                ++len;

            return len;
        }

        static char_type const * find(char_type * const p, pie::size_t count, char_type const & ch)
        {
            for (pie::size_t i = 0; i < count; i++)
            {
                if (eq(p[i], ch) == true)
                    return p + i;
            }

            return nullptr;
        }

        /*static char_type to_char_type(int_type i)
        {
            return static_cast<char_type>(i);
        }*/
    };

    template<class String>
    class string_iterator : public pie::iterator<random_access_iterator_tag, typename String::value_type>
    {
    public:
        typedef string_iterator<String> iterator;

        typedef typename String::value_type value_type;
        typedef typename String::difference_type difference_type;
        typedef typename String::pointer pointer;
        typedef typename String::reference reference;

        string_iterator() : m_ptr() { }

        string_iterator(pointer ptr) : m_ptr(ptr) { }

        reference operator*() const
        {
            return *m_ptr;
        }

        pointer operator->() const
        {
            return pointer_traits<pointer>::pointer_to(**this);
        }

        iterator& operator++()
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

            return tmp;
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
            return *(m_ptr + offset);
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
            return m_ptr <= it.m_ptr;
        }

        bool operator>=(iterator const & it) const
        {
            return m_ptr >= it.m_ptr;
        }

        pointer m_ptr;
    };

    template<class String>
    inline string_iterator<String> operator+(typename string_iterator<String>::difference_type offset,
                                             string_iterator<String> next)
    {
        return next += offset;
    }

    template<class String>
    inline string_iterator<String> operator-(typename string_iterator<String>::difference_type offset,
                                             string_iterator<String> next)
    {
        return next -= offset;
    }

    template<class String>
    class string_const_iterator : public pie::iterator<random_access_iterator_tag, typename String::value_type>
    {
    public:
        typedef string_const_iterator<String> iterator;

        typedef typename String::value_type value_type;
        typedef typename String::difference_type difference_type;
        typedef typename String::const_pointer pointer;
        typedef typename String::const_reference reference;

        string_const_iterator() : m_ptr() { }

        string_const_iterator(pointer ptr) : m_ptr(ptr) { }

        string_const_iterator(string_iterator<String> const & it) : m_ptr(it.m_ptr) { }

        reference operator*() const
        {
            return *m_ptr;
        }

        pointer operator->() const
        {
            return pointer_traits<pointer>::pointer_to(**this);
        }

        iterator& operator++()
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

            return tmp;
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
            return *(m_ptr + offset);
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
            return m_ptr <= it.m_ptr;
        }

        bool operator>=(iterator const & it) const
        {
            return m_ptr >= it.m_ptr;
        }

        pointer m_ptr;
    };

    template<class String>
    inline string_const_iterator<String> operator+(typename string_const_iterator<String>::difference_type offset,
                                                   string_const_iterator<String> next)
    {
        return next += offset;
    }

    template<class String>
    inline string_const_iterator<String> operator-(typename string_const_iterator<String>::difference_type offset,
                                                   string_const_iterator<String> next)
    {
        return next -= offset;
    }

    template<class CharType, class Traits = pie::char_traits<CharType, pie::size_t>, class Allocator = pie::allocator<CharType>>
    class basic_string 
    {
    public:
        typedef Traits traits_type;
        typedef typename Traits::char_type value_type;
        typedef Allocator allocator_type;
        typedef typename Allocator::size_type size_type ;
        typedef typename Allocator::difference_type difference_type;
        typedef typename Allocator::reference reference;
        typedef typename Allocator::const_reference const_reference;
        typedef typename Allocator::pointer pointer;
        typedef typename Allocator::const_pointer const_pointer;
        //typedef pointer iterator;
        typedef string_iterator<basic_string<value_type>> iterator;
        // typedef const_pointer const_iterator;
        typedef string_const_iterator<basic_string<value_type>> const_iterator;
        typedef pie::reverse_iterator<iterator> reverse_iterator;
        typedef pie::reverse_iterator<const_iterator> const_reverse_iterator;
        
        basic_string() :
            m_allocator(),
            m_string(),
            m_size(),
            m_capacity()
        {
        }

        basic_string(basic_string const & str) :
            m_allocator(),
            m_string(),
            m_size(),
            m_capacity()
        {
            assign(str);
        }

        basic_string(basic_string const & str, allocator_type const & alloc) :
            m_allocator(alloc),
            m_string(),
            m_size(),
            m_capacity()
        {
            assign(str);
        }

        basic_string(basic_string && str) :
            m_allocator(),
            m_string(),
            m_size(),
            m_capacity()
        {
            assign(pie::move(str));
        }

        basic_string(basic_string && str, allocator_type const & alloc) :
            m_allocator(alloc),
            m_string(),
            m_size(),
            m_capacity()
        {
            assign(pie::move(str));
        }

        basic_string(size_type count, value_type ch) :
            m_allocator(),
            m_string(),
            m_size(),
            m_capacity()
        {
            assign(count, ch);
        }

        basic_string(size_type count, value_type ch, allocator_type const & alloc) :
            m_allocator(alloc),
            m_string(),
            m_size(),
            m_capacity()
        {
            assign(count, ch);
        }

        basic_string(value_type const * str) :
            m_allocator(),
            m_string(),
            m_size(),
            m_capacity()
        {
            assign(str);
        }

        basic_string(value_type const * str, allocator_type const & alloc) :
            m_allocator(alloc),
            m_string(),
            m_size(),
            m_capacity()
        {
            assign(str);
        }
        
        basic_string(value_type const * str, size_type count) :
            m_allocator(),
            m_string(),
            m_size(),
            m_capacity()
        {
            assign(str, count);
        }

        basic_string(value_type const * str, size_type count, allocator_type const & alloc) :
            m_allocator(alloc),
            m_string(),
            m_size(),
            m_capacity()
        {
            assign(str, count);
        }

        virtual ~basic_string()
        {
            cleanup();
        }

        basic_string & operator=(basic_string const & str)
        {
            assign(str);

            return *this;
        }

        basic_string & operator=(basic_string && str)
        {
            assign(pie::move(str));

            return *this;
        }

        basic_string & operator=(value_type * cstr)
        {
            assign(cstr);

            return *this;
        }

        reference operator[](size_type pos)
        {
            return at(pos);
        }

        const_reference operator[](size_type pos) const
        {
            return at(pos);
        }

        // Replaces the contents with count copies of character ch.
        basic_string & assign(size_type count, value_type ch)
        {
            resize(count);

            for (size_type i = 0; i < count; i++)
            {
                m_string[i] = ch;
            }
            
            return *this;
        }

        // Replaces the contents with a copy of str.
        basic_string & assign(basic_string const & str)
        {
            resize(str.m_size);

            for (size_type i = 0; i < str.m_size; ++i)
            {
                m_string[i] = str.m_string[i];
            }

            return *this;
        }

        // Replaces the contents with those of str using move semantics. Equivalent to *this = std::move(str)
        basic_string & assign(basic_string && str)
        {
            if (m_string == str.m_string)
                return *this;

            cleanup();

            m_string = str.m_string;
            m_size = str.m_size;
            m_capacity = str.m_capacity;
            str.m_string = nullptr;
            str.m_size = 0;
            str.m_capacity = 0;

            return *this;
        }

        // Replaces the contents with the first count characters of character string pointed to by s. s can contain null characters.
        basic_string & assign(value_type const * str, size_type count)
        {
            resize(count);
            if (m_string == nullptr)
                return *this;

            m_size = count;
            for (size_type i = 0; i < count; i++)
            {
                m_string[i] = str[i];
            }
            
            return *this;
        }

        // Replaces the contents with those of null-terminated character string pointed to by s. The length of the string is determined by the first null character.
        basic_string & assign(value_type const * str)
        {
            assign(str, traits_type::length(str));

            return *this;
        }

        //Replaces the contents with copies of the characters in the range [first, last). This overload does not participate in overload resolution if InputIt does not satisfy InputIterator.
        template<class InputIterator>
        basic_string & assign(InputIterator first, InputIterator last)
        {
            cleanup();

            replace(begin(), end(), first, last);

            return *this;
        }

        // Replaces the contents with those of the initializer list init_list.
        basic_string & assign(std::initializer_list<value_type> const init_list)
        {
            cleanup();

            replace(begin(), end(), init_list..);

            return *this;
        }

        reference at(size_type pos)
        {
            static value_type empty;

            return pos < m_size + m_capacity ? m_string[pos] : empty;
        }

        const_reference at(size_type pos) const
        {
            static value_type empty;

            return pos < m_size + m_capacity ? m_string[pos] : empty;
        }

        iterator begin()
        {
            return iterator(m_string);
        }

        const_iterator begin() const
        {
            return const_iterator(m_string);
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(cend());
        }

        const_iterator cbegin() const
        {
            return const_iterator(begin());
        }

        const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator(cend());
        }

        iterator end()
        {
            return iterator(m_string + m_size);
        }

        const_iterator end() const
        {
            return const_iterator(m_string + m_size);
        }

        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(cbegin());
        }

        const_iterator cend() const
        {
            return const_iterator(end());
        }

        const_reverse_iterator crend() const
        {
            return const_reverse_iterator(cbegin());
        }

        value_type const * c_str() const
        {
            return m_string;
        }

        value_type * data()
        {
            return m_string;
        }

        value_type const * data() const
        {
            return m_string;
        }

        bool empty() const
        {
            return m_string == nullptr;
        }

        void clear()
        {
            cleanup();
        }

        int compare(basic_string const & str) const
        {
            return traits_type::compare(m_string, str.c_str(), str.length());
        }

        int compare(value_type const * cstr) const
        {
            return traits_type::compare(m_string, cstr, traits_type::length(cstr));
        }

        size_type length() const
        {
            return traits_type::length(m_string);
        }

        size_type capacity() const
        {
            return m_size + m_capacity;
        }

        void resize(size_type count)
        {
            if (count > m_capacity)
            {
                cleanup();
                
                m_string = m_allocator.allocate(count + m_reserve + 1);
                if (m_string != nullptr)
                {
                    m_size = count;
                    m_capacity = count + m_reserve;
                    m_string[count] = value_type();
                }
            }
            
        }

        size_type size() const
        {
            return m_size;
        }

        size_type max_size() const
        {
            return m_allocator.max_size();
        }

        basic_string & append(size_type count, value_type ch)
        {
            resize_copy(m_size + count);
            if (m_string == nullptr)
                return *this;

            traits_type::assign(m_string + m_size /* -1 */, count, ch);
            m_size += count;

            return *this;
        }

        basic_string & append(value_type const * cstr)
        {
            size_type len = traits_type::length(cstr);
            resize_copy(m_size + len);
            if (m_string == nullptr)
                return *this;

            traits_type::copy(m_string + m_size, cstr, len);
            m_size += len;

            return *this;
        }

        //basic_string & append(basic_string const & str)
        //{
        //    return *this;
        //}

        //basic_string & append(value_type const * cstr, size_type count)
        //{
        //    return *this;
        //}

        //template<class InputIterator>
        //basic_string & append(InputIterator first, InputIterator last)
        //{
        //    return *this;
        //}

        //basic_string & append(std::initializer_list<value_type> init_list)
        //{
        //    return *this;
        //}

        basic_string & insert(size_type index, size_type count, value_type ch)
        {
            if (index > size())
                return *this; // out_of_range

            if (m_size + count > max_size())
                return *this; // out_of_range
            
            // Reallocate if necessary
            resize_copy(m_size + count);
            if (m_string == nullptr)
                return *this;
            // Shift the elements
            traits_type::move(m_string + index + count, m_string + index, m_size + count);
            // Insert the elements
            traits_type::assign(m_string + index, count, ch);
            
            m_size += count;
            m_string[m_size] = value_type();

            return *this;
        }

        basic_string & insert(size_type index, value_type const * cstr)
        {
            if (index > size())
                return *this;

            size_type len = traits_type::length(cstr);
            if (m_size + len > max_size())
                return *this;

            resize_copy(m_size + len);
            if (m_string == nullptr)
                return *this;
            
            traits_type::move(m_string + index + len, m_string + index, m_size + len);
            traits_type::copy(m_string + index, cstr, len);

            m_size += len;
            m_string[m_size] = value_type();

            return *this;
        }

        //basic_string & insert(size_type index, value_type const * cstr, size_type count)
        //{

        //}

        basic_string & insert(size_type index, basic_string const & str)
        {
            return insert(index, str.c_str());
        }

        //basic_string & insert(size_type index, basic_string const & str, size_type index_str, size_type count)
        //{

        //}

        iterator insert(iterator pos, value_type ch)
        {
            size_type offset = pos - begin();
            insert(offset, 1, ch);

            return pos + offset;
        }

        /*iterator insert(const_iterator pos, size_type count, value_type ch)
        {
        }*/

        basic_string & erase(size_type index = 0, size_type count = npos)
        {
            if (index > m_size)
                return *this; // out_of_range

            traits_type::move(m_string + index, m_string + index + count, index + count);
            m_size -= count;

            return *this;
        }

        // Removes min(count, size() - index) characters starting at index.
        iterator erase(iterator position)
        {
            return iterator();
        }

        iterator erase(const_iterator position)
        {

        }

        // Removes the character at position.
        iterator erase(iterator first, iterator last)
        {
            return iterator();
        }

        // Removes the characters in the range [first, last).
        iterator erase(const_iterator first, const_iterator last)
        {
            
        }

        void push_back(value_type ch)
        {
            append(1, ch);
        }

        void pop_back()
        {
            erase(m_size, 1);
        }

        // Replaces the string with the range [pos, pos + count) of str
        //basic_string & replace(size_type pos, size_type count, basic_string const & str)
        //{
        //    return *this;
        //}

        //// Replaces the range [first, last) with the range [str.begin(), str.end()) of str
        //basic_string & replace(const_iterator first, const_iterator last, basic_string const & str)
        //{
        //    return *this;
        //}

        //// Replaces the range [first, last) with the range [first2, last2)
        //template<class InputIterator>
        //basic_string & replace(const_iterator first, const_iterator last, InputIterator first2, InputIterator last2)
        //{
        //    return *this;
        //}

        //// Replaces the range [pos, pos + count) with [cstr, cstr + count)
        //basic_string & replace(size_type pos, size_type count, value_type const * cstr, size_type count2)
        //{
        //    return *this;
        //}

        //// Replaces the range [first, last) with [cstr, cstr + count)
        //basic_string & replace(const_iterator first, const_iterator last, value_type const * cstr, size_type count)
        //{
        //    return *this;
        //}

        //basic_string & replace(size_type pos, size_type count, value_type const * cstr)
        //{
        //    return *this;
        //}

        //basic_string & replace(const_iterator first, const_iterator last, value_type const * cstr)
        //{
        //    return *this;
        //}

        //basic_string & replace(size_type pos, size_type count, size_type count2, value_type ch)
        //{
        //    return *this;
        //}

        //basic_string & replace(const_iterator first, const_iterator last, size_type count, value_type ch)
        //{
        //    return *this;
        //}

        //basic_string & replace(const_iterator first, const_iterator last, pie::initializer_list<value_type> init_list)
        //{
        //    return *this;
        //}
        //} 
        
        static size_type const npos = -1;
    protected:
    private:
        value_type * m_string;
        size_type m_size;
        size_type m_capacity;
        size_type m_reserve = 32;
        allocator_type m_allocator;

        void cleanup()
        {
            if (m_string != nullptr)
            {
                m_allocator.deallocate(m_string);
                m_string = nullptr;
            }
        }

        void resize_copy(size_type count)
        {
            if (count > m_size)
            {
                value_type * new_string = m_allocator.allocate(count + m_reserve + 1);
                if (new_string != nullptr)
                {
                    pie::copy(begin(), end(), new_string);

                    m_allocator.deallocate(m_string);

                    new_string[count] = value_type();
                    m_string = new_string;
                    m_capacity = m_reserve + count;
                }
            }
        }

        //void shift_elements(iterator first, iterator last, size_type count)
        //{
        //    // Make sure the shift will be within the bounds of our capacity
        //    if (end() + count < begin() + m_capacity)
        //    {

        //    }
        //}
    };

    template<class CharType, class Traits, class Allocator>
    bool operator==(basic_string<CharType, Traits, Allocator> const & left,
                           basic_string<CharType, Traits, Allocator> const & right)
    {
        return left.compare(right) == 0;
    }

    template<class CharType, class Traits, class Allocator>
    bool operator!=(basic_string<CharType, Traits, Allocator> const & left,
                           basic_string<CharType, Traits, Allocator> const & right)
    {
        return left.compare(right) != 0;
    }

    template<class CharType, class Traits, class Allocator>
    bool operator<(basic_string<CharType, Traits, Allocator> const & left,
                          basic_string<CharType, Traits, Allocator> const & right)
    {
        return left.compare(right) == -1;
    }

    template<class CharType, class Traits, class Allocator>
    bool operator>(basic_string<CharType, Traits, Allocator> const & left,
                          basic_string<CharType, Traits, Allocator> const & right)
    {
        return left.compare(right) == 1;
    }

    template<class CharType, class Traits, class Allocator>
    bool operator<=(basic_string<CharType, Traits, Allocator> const & left,
                           basic_string<CharType, Traits, Allocator> const & right)
    {
        return left.compare(right) <= 0;
    }

    template<class CharType, class Traits, class Allocator>
    bool operator>=(basic_string<CharType, Traits, Allocator> const & left,
                           basic_string<CharType, Traits, Allocator> const & right)
    {
        return left.compare(right) >= 0;
    }

    template< class CharType, class Traits, class Alloc >
    bool operator==(CharType const * left, basic_string<CharType,Traits,Alloc> const & right)
    {
        return right.compare(left) == 0;
    }

    template< class CharType, class Traits, class Alloc >
    bool operator==(basic_string<CharType,Traits,Alloc> const & left, CharType const * right)
    {
        return left.compare(right) == 0;
    }

    template< class CharType, class Traits, class Alloc >
    bool operator!=(CharType const * left, basic_string<CharType,Traits,Alloc> const & right)
    {
        return right.compare(left) != 0;
    }

    template< class CharType, class Traits, class Alloc >
    bool operator!=(basic_string<CharType,Traits,Alloc> const & left, CharType const * right)
    {
        return left.compare(right) != 0;
    }

    template< class CharType, class Traits, class Alloc >
    bool operator<(CharType const * left, basic_string<CharType,Traits,Alloc> const & right)
    {
        return right.compare(left) == 1;
    }

    template< class CharType, class Traits, class Alloc >
    bool operator<(basic_string<CharType,Traits,Alloc> const & left,  CharType const * right)
    {
        return left.compare(right) == -1;
    }

    template< class CharType, class Traits, class Alloc >
    bool operator>(CharType const * left, basic_string<CharType,Traits,Alloc> const & right)
    {
        return right.compare(left) == -1;
    }
  
    template< class CharType, class Traits, class Alloc >
    bool operator>(basic_string<CharType,Traits,Alloc> const& left, CharType const * right)
    {
        return left.compare(right) == 1;
    }

    template< class CharType, class Traits, class Alloc >
    bool operator<=(CharType const* left, basic_string<CharType,Traits,Alloc> const & right)
    {
        return right.compare(left) >= 0;
    }
   
    template< class CharType, class Traits, class Alloc >
    bool operator<=(basic_string<CharType,Traits,Alloc> const & left, CharType const * right)
    {
        return left.compare(right) <= 0;
    }
    
    template< class CharType, class Traits, class Alloc >
    bool operator>=(CharType const * left, basic_string<CharType,Traits,Alloc> const & right)
    {
        return right.compare(left) <= 0;
    }

    template< class CharType, class Traits, class Alloc >
    bool operator>=(const basic_string<CharType,Traits,Alloc>& left, CharType const * right)
    {
        return left.compare(right) >= 0;
    }

    template<class CharType, class Traits, class Alloc>
    basic_string<CharType, Traits, Alloc> operator+(basic_string<CharType, Traits, Alloc> const & left,
                                                    basic_string<CharType, Traits, Alloc> const & right)
    {
        return basic_string<CharType, Traits, Alloc>(left).append(right);
    }

    template<class CharType, class Traits, class Alloc>
    basic_string<CharType, Traits, Alloc> operator+(CharType const * left,
                                                    basic_string<CharType, Traits, Alloc> const & right)
    {
        return basic_string<CharType, Traits, Alloc>(left) + right;
    }

    template<class CharType, class Traits, class Alloc>
    basic_string<CharType, Traits, Alloc> operator+(CharType left,
                                                    basic_string<CharType, Traits, Alloc> const & right)
    {
        return basic_string<CharType, Traits, Alloc>(1, left) + right;
    }

    template<class CharType, class Traits, class Alloc>
    basic_string<CharType, Traits, Alloc> operator+(basic_string<CharType, Traits, Alloc> const & left,
                                                    CharType const * right)
    {
        return left + basic_string<CharType, Traits, Alloc>(right);
    }

    template<class CharType, class Traits, class Alloc>
    basic_string<CharType, Traits, Alloc> operator+(basic_string<CharType, Traits, Alloc> const & left,
                                                    CharType right)
    {
        return left + basic_string<CharType, Traits, Alloc>(1, right);
    }

    template<class CharType, class Traits, class Alloc>
    basic_string<CharType, Traits, Alloc> operator+(basic_string<CharType, Traits, Alloc> && left,
                                                    basic_string<CharType, Traits, Alloc> const & right)
    {
        return pie::move(left.append(right));
    }

    template<class CharType, class Traits, class Alloc>
    basic_string<CharType, Traits, Alloc> operator+(basic_string<CharType, Traits, Alloc> const & left,
                                                    basic_string<CharType, Traits, Alloc> && right)
    {
        return pie::move(right.insert(0, left));
    }

    template<class CharType, class Traits, class Alloc>
    basic_string<CharType, Traits, Alloc> operator+(basic_string<CharType, Traits, Alloc> && left,
                                                    basic_string<CharType, Traits, Alloc> && right)
    {
        return pie::move(left.append(right));
    }

    template<class CharType, class Traits, class Alloc>
    basic_string<CharType, Traits, Alloc> operator+(CharType const * left,
        basic_string<CharType, Traits, Alloc> && right)
    {
        return pie::move(right.insert(0, left));
    }

    template<class CharType, class Traits, class Alloc>
    basic_string<CharType, Traits, Alloc> operator+(CharType left,
                                                    basic_string<CharType, Traits, Alloc> && right)
    {
        return pie::move(right.insert(0, 1, left));
    }

    template<class CharType, class Traits, class Alloc>
    basic_string<CharType, Traits, Alloc> operator+(basic_string<CharType, Traits, Alloc> && left,
                                                    CharType const * right)
    {
        return pie::move(left.append(right));
    }

    template<class CharType, class Traits, class Alloc>
    basic_string<CharType, Traits, Alloc> operator+(basic_string<CharType, Traits, Alloc> && left,
                                                    CharType right)
    {
        return pie::move(left.append(1, right));
    }

    typedef basic_string<char> string;
    typedef basic_string<wchar_t> wstring;
}

#endif
