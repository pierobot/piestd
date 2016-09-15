#pragma once

#include <pie/utility.hpp>

namespace pie
{
	template<class InputIterator, class OutputIterator>
	InputIterator copy(InputIterator first, InputIterator last, OutputIterator dest)
	{
		while (first != last)
			*dest++ = *first++;

		return (dest);
	}

	template<class InputIterator, class OutputIterator, class UnaryPredicate>
	InputIterator copy_if(InputIterator first, InputIterator last, OutputIterator dest, UnaryPredicate pred)
	{
		while (first != last)
			if (pred(*first))
				*dest++ = *first++;

		return (dest);
	}

	template<class InputIterator, class Size, class OutputIterator>
	InputIterator copy_n(InputIterator first, Size count, OutputIterator dest)
	{
		for (Size i = 0; i < count; i++)
			*dest++ = *first++;

		return (dest);
	}

	template<class InputIterator, class Value>
	InputIterator find(InputIterator first, InputIterator last, Value const & value)
	{
		for (InputIterator & current = first; current != last; ++current)
		{
			if (*current == value)
				return (current);
		}

		return (last);
	}

	template<class InputIterator, class UnaryPredicate>
	InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate pred)
	{
		for (InputIterator & current = first; current != last; ++current)
		{
			if (pred(*current))
				return (current);
		}

		return (last);
	}

	template<class InputIterator, class UnaryPredicate>
	InputIterator find_if_not(InputIterator first, InputIterator last, UnaryPredicate pred)
	{
		for (InputIterator & current = first; current != last; ++current)
		{
			if (!pred(*current))
				return (current);
		}

		return (last);
	}

	template<class InputIterator, class UnaryPredicate>
	UnaryPredicate for_each(InputIterator first, InputIterator last, UnaryPredicate pred)
	{
		for (InputIterator & current = first; current != last; ++current)
		{
			pred(*current);
		}

		return (func);
	}

	template<class ForwardIterator1, class ForwardIterator2>
	void iter_swap(ForwardIterator1 left, ForwardIterator2 right)
	{
		pie::swap(*left, *right);
	}

	template<class ForwardIterator>
	void rotate(ForwardIterator first, ForwardIterator n_first, ForwardIterator last)
	{
		ForwardIterator next = n_first;
		while (first != last)
		{
			pie::iter_swap(first++, next++);

			if (next == last)
			{
				next = n_first;
			}
			else if (first == n_first)
			{
				n_first = next;
			}
		}
	}

};
