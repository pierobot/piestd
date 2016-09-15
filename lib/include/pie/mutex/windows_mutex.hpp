#pragma once

#include <Windows.h>

namespace pie
{
	class mutex
	{
	public:
		typedef void * native_handle_type;

		mutex() noexcept :
			m_section()
		{
			::InitializeCriticalSection(&m_section);
		}

		mutex(mutex const &) = delete;

		~mutex()
		{
			::DeleteCriticalSection(&m_section);
		}

		mutex & operator=(mutex const &) = delete;

		void lock() // throws system_error object
		{
			::EnterCriticalSection(&m_section);
		}

		bool try_lock() noexcept
		{
			return ::TryEnterCriticalSection(&m_section) == TRUE;
		}

		void unlock() noexcept
		{
			::LeaveCriticalSection(&m_section);
		}

		native_handle_type native_handle() noexcept
		{
			return static_cast<native_handle_type>(&m_section);
		}
	protected:
	private:
		CRITICAL_SECTION m_section;
	};
}