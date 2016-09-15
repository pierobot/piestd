#pragma once

namespace pie
{
	template<class T> struct add_volatile { typedef volatile T type; };
}
