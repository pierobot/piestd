#pragma once

#include <pie/type_traits/integral_constant.hpp>
#include <pie/type_traits/is_integral.hpp>

namespace pie
{
	template<class T, T... Vals>
	struct integer_sequence
	{
		static_assert(is_integral<T>::value, "T must be an integral type.");

		typedef integer_sequence<T, Vals...> type;
		typedef T value_type;

		static constexpr size_t size() noexcept
		{
			return sizeof...(Vals);
		}
	};

#ifdef _MSC_VER
	template<class T, T Size>
	using make_integer_sequence = __make_integer_seq<integer_sequence, T, Size>;
#else
	namespace detail
	{
		template<bool Negative, bool Zero, class IntegralConstant, class IntegerSequence>
		struct make_sequence
		{
			static_assert(!Negative, "make_sequence requires N to be non-negative.");
		};

		template<class T, T... Vals>
		struct make_sequence<false, true,
			integral_constant<T, 0>,
			integer_sequence<T, Vals...>> : integer_sequence<T, Vals...>
		{

		};

		template<class T, T Index, T... Vals>
		struct make_sequence<false, false,
			integral_constant<T, Index>,
			integer_sequence<T, Vals...>> : make_sequence<false, Index == 1, integral_constant<T, Index - 1>, integer_sequence<T, Index - 1, Vals...>>
		{
		};

	}

	template<class T, T Size>
	using make_integer_sequence = typename detail::make_sequence<Size < 0, Size == 0, integral_constant<T, Size>, integer_sequence<T>>::type;

#endif

	template<pie::size_t... Vals>
	using index_sequence = integer_sequence<pie::size_t, Vals...>;

	template<pie::size_t Size>
	using make_index_sequence = make_integer_sequence<pie::size_t, Size>;

	template<class... Types>
	using index_sequence_for = make_index_sequence<sizeof...(Types)>;
}