#pragma once

#include <pie/stdint.hpp>

namespace pie
{
    struct input_iterator_tag { };
    struct output_iterator_tag { };
    struct forward_iterator_tag : public input_iterator_tag { };
    struct bidirectional_iterator_tag : public forward_iterator_tag { };
    struct random_access_iterator_tag : public bidirectional_iterator_tag { };

    template<class Iterator>
    struct iterator_traits
    {
        using difference_type = typename Iterator::difference_type;
        using value_type = typename Iterator::value_type;
        using pointer = typename Iterator::pointer;
        using reference = typename Iterator::reference;
        using iterator_category = typename Iterator::iterator_category;
    };

    template<class T>
    struct iterator_traits<T *>
    {
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = pie::random_access_iterator_tag;
    };

    template<class T>
    struct iterator_traits<T const *>
    {
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T const*;
        using reference = T const&;
        using iterator_category = pie::random_access_iterator_tag;
    };

    template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
    struct iterator
    {
        typedef Category iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef Pointer pointer;
        typedef Reference reference;
    };

    template<class InputIterator>
    typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last)
    {
        return last - first;
    }

    template<class InputIterator, class Distance>
    void advance(InputIterator & it, Distance n)
    {
        while (n-- > 0)
            ++it;
    }

    template<class BiDirectionalIterator>
    BiDirectionalIterator prev(BiDirectionalIterator it, typename pie::iterator_traits<BiDirectionalIterator>::difference_type n = 1)
    {
        pie::advance(it, -n);

        return it;
    }

    template<class ForwardIterator>
    ForwardIterator next(ForwardIterator it, typename pie::iterator_traits<ForwardIterator>::difference_type n = 1)
    {
        pie::advance(it, n);

        return it;
    }

    template<class Iterator>
    class reverse_iterator : public pie::iterator<typename pie::iterator_traits<Iterator>::iterator_category,
                                                  typename pie::iterator_traits<Iterator>::value_type,
                                                  typename pie::iterator_traits<Iterator>::difference_type,
                                                  typename pie::iterator_traits<Iterator>::pointer,
                                                  typename pie::iterator_traits<Iterator>::reference>
    {
    public:
        typedef Iterator iterator_type;

        reverse_iterator() : m_it() { }

        explicit reverse_iterator(Iterator it) : m_it(it) { }

        template<class U>
        reverse_iterator(reverse_iterator<U> const & it) : m_it(it.m_it) { }

        iterator_type base() const
        {
            return m_it;
        }

        reference operator*() const
        {
            iterator_type tmp = m_it;
            return *--tmp;
        }

        pointer operator->() const
        {
            return pie::pointer_traits<pointer>::pointer_to(**this);
        }

        template<class U>
        reverse_iterator & operator=(reverse_iterator<U> const & it)
        {
            m_it = it.base();
            return *this;
        }

        reference operator[](difference_type n) const
        {
            iterator_type tmp = m_it;

            return *--tmp;
        }

        reverse_iterator & operator++()
        {
            --m_it;

            return *this;
        }

        reverse_iterator operator++(int)
        {
            reverse_iterator tmp = *this;
            --m_it;

            return tmp;
        }

        reverse_iterator & operator--()
        {
            ++m_it;

            return *this;
        }

        reverse_iterator operator--(int)
        {
            reverse_iterator tmp = *this;
            ++m_it;

            return tmp;
        }

        reverse_iterator operator+(difference_type n) const
        {
            return reverse_iterator(m_it - n);
        }

        reverse_iterator operator-(difference_type n) const
        {
            return reverse_iterator(m_it + n);
        }

        reverse_iterator operator+=(difference_type n)
        {
            current -= n;

            return *this;
        }

        reverse_iterator operator-=(difference_type n)
        {
            m_it += n;

            return *this;
        }

        iterator_type m_it;
    };

    template<class Iterator1, class Iterator2>
    bool operator==(reverse_iterator<Iterator1> const & left, reverse_iterator<Iterator2> const & right)
    {
        return left.base() == right.base();
    }

    template<class Iterator1, class Iterator2>
    bool operator!=(reverse_iterator<Iterator1> const & left, reverse_iterator<Iterator2> const & right)
    {
        return left.base() != right.base();
    }

    template<class Iterator1, class Iterator2>
    bool operator<(reverse_iterator<Iterator1> const & left, reverse_iterator<Iterator2> const & right)
    {
        return left.base() > right.base();
    }

    template<class Iterator1, class Iterator2>
    bool operator<=(reverse_iterator<Iterator1> const & left, reverse_iterator<Iterator2> const & right)
    {
        return left.base() >= right.base();
    }

    template<class Iterator1, class Iterator2>
    bool operator>(reverse_iterator<Iterator1> const & left, reverse_iterator<Iterator2> const & right)
    {
        return left.base() < right.base();
    }

    template<class Iterator1, class Iterator2>
    bool operator>=(reverse_iterator<Iterator1> const & left, reverse_iterator<Iterator2> const & right)
    {
        return left.base() <= right.base();
    }

    template<class Iterator>
    reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n, reverse_iterator<Iterator> & it)
    {
        return pie::prev(it, n);
    }

    template<class Iterator>
    typename reverse_iterator<Iterator>::difference_type operator-(reverse_iterator<Iterator> const & left, 
                                                                   reverse_iterator<Iterator> const & right)
    {
        return right.base() - left.base();
    }

    template<class Iterator>
    auto operator-(reverse_iterator<Iterator> const & left, reverse_iterator<Iterator> const & right) -> decltype(right.base() - left.base())
    {
        return right.base() - left.base();
    }

}
