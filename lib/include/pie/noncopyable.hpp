#pragma once

namespace pie
{
	class noncopyable
	{
	public:
		noncopyable() = default;
		noncopyable(noncopyable const & ) = delete;
		
		noncopyable & operator=(noncopyable const &) = delete;
	protected:
	private:
	};
}
