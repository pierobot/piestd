#pragma once

#include <pie/string.hpp>

namespace pie
{
    enum fmtflags
    {

    };

    class ios_base;

    template<class CharType, class Traits = pie::char_traits<CharType>>
    class ios_base
    {
    public:
        ios_base(ios_base const &) = delete;
    protected:
        ios_base() { }
    private:
    };
}
