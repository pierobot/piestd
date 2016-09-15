#pragma once

namespace pie
{
	template<bool Bool, class TrueType, class FalseType>
	struct conditional { typedef TrueType type; };

	template<class TrueType, class FalseType>
	struct conditional<false, TrueType, FalseType> { typedef FalseType type; };
}
