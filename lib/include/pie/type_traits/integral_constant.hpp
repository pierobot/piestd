#pragma once

namespace pie
{
	template<class Type, Type val>
	struct integral_constant
	{
		static Type const value = val;
		typedef Type value_type;
		typedef integral_constant type;
		operator value_type() const noexcept { return value; }
		constexpr value_type operator()() const noexcept { return value; }
	};

	typedef pie::integral_constant<bool, true> true_type;
	typedef pie::integral_constant<bool, false> false_type;
}
