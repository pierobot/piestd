#pragma once

#include <pie/threads.h>

namespace pie
{
	class mutex
	{
	public:
		typedef mtx_t * native_handle_type;

		mutex() noexcept
		{
			mtx_init(&m_mtx, mtx_plain);
		}

		mutex(mutex const &) = delete;

		~mutex()
		{
			mtx_destroy(&m_mtx);
		}

		mutex & operator=(mutex const &) = delete;

		void lock() // throws system_error object
		{
			mtx_lock(&m_mtx);
		}

		bool try_lock() noexcept
		{
			return mtx_trylock(&m_mtx) == thrd_success;
		}

		void unlock() noexcept
		{
			mtx_unlock(&m_mtx);
		}

		native_handle_type native_handle() noexcept
		{
			return &m_mtx;
		}
	protected:
	private:
		mtx_t m_mtx;
	};

	struct defer_lock_t {}; // do not acquire ownership of the mutex
	struct try_to_lock_t {}; // try to acquire ownership of the mutex without blocking
	struct adopt_lock_t {}; // assume the calling thread already has ownership of the mutex

	template<class Mutex>
	class lock_guard
	{
	public:
		typedef Mutex mutex_type;

		explicit lock_guard(mutex_type & mutex) :
			m_mutex(mutex)
		{
			m_mutex.lock();
		}

		explicit lock_guard(mutex_type & mutex, pie::adopt_lock_t t) noexcept :
			m_mutex(mutex)
		{
		}

		lock_guard(lock_guard const &) = delete;

		~lock_guard()
		{
			m_mutex.unlock();
		}

		lock_guard & operator=(lock_guard const &) = delete;
	protected:
	private:
		pie::mutex & m_mutex;
	};

	template<class Mutex>
	class unique_lock
	{
	public:
		typedef Mutex mutex_type;

		unique_lock() noexcept : 
			m_mutex(nullptr),
			m_owns(false)
		{
		}

		unique_lock(unique_lock && other) noexcept :
			m_mutex(other.m_mutex),
			m_owns(other.m_owns)
		{
			other.m_mutex = nullptr;
			other.m_owns = false;
		}

		explicit unique_lock(mutex_type & mtx) :
			m_mutex(pie::addressof(mtx)),
			m_owns(false)
		{
			m_mutex->lock();
			m_owns = true;
		}

		unique_lock(mutex_type & mtx, pie::defer_lock_t) noexcept :
			m_mutex(pie::addressof(mtx)),
			m_owns(false)
		{
		}

		unique_lock(mutex_type & mtx, pie::try_to_lock_t) :
			m_mutex(pie::addressof(mtx)),
			m_owns(m_mutex->try_lock())
		{
		}

		unique_lock(mutex_type & mtx, pie::adopt_lock_t) :
			m_mutex(pie::addressof(mtx)),
			m_owns(true)
		{
		}

		// pie::chrono unimplemented 
		/*template<class Rep, class Period>
		unique_lock(mutex_type & mtx, pie::chrono::duration<Rep, Period> const & timeout_duration)
		{
		}
		}*/

		// pie::chrono unimplemented
		/*template<class Clock, class Duration>
		unique_lock(mutex_type & mtx, pie::chrono::time_point<Clock, Duration> const & timeout_time)
		{
		}*/

		~unique_lock()
		{
			if (m_owns == true && m_mutex != nullptr)
				m_mutex->unlock();
		}

		void swap(unique_lock & other) noexcept
		{
			pie::swap(m_mutex, other.m_mutex);
			pie::swap(m_owns, other.m_owns);
		}

		void lock()
		{
			if (m_mutex == nullptr)
			{
				// throw system_error with ec = errc::operation_not_permitted
				return;
			}

			if (m_owns == true)
			{
				// throw system_error with ec = errc::resource_deadlock_would_occur
				return;
			}

			m_mutex->lock();
			m_owns = true;
		}

		bool try_lock()
		{
			if (m_mutex == nullptr)
			{
				// throw system_error with ec = errc::operation_not_permitted
				return false;
			}

			if (m_owns == true)
			{
				// throw system_error with ec = errc::resource_deadlock_would_occur
				return false;
			}

			if (m_mutex->try_lock() == true)
			{
				m_owns = true;
				return true;
			}

			return false;
		}

		/*template<class Rep, class Period>
		bool try_lock_for(pie::chrono::duration<Rep, Period> const & timeout_duration)
		{

		}*/

		/*template<class Clock, class Duration>
		bool try_lock_until(pie::chrono::time_point<Clock, Duration> const & timeout_time)
		{

		}*/

		void unlock()
		{
			if (m_mutex == nullptr && m_owns == false)
			{
				// throw system_error with ec = errc::operation_not_permitted
				return;
			}
		}

		mutex_type * release() noexcept
		{
			mutex_type * tmp_mutex = m_mutex;
			m_owns = false;
			m_mutex = nullptr;

			return tmp_mutex;
		}

		mutex_type * mutex() const noexcept
		{
			return m_mutex;
		}

		bool owns_lock() const noexcept
		{
			return m_owns;
		}

		operator bool() const noexcept
		{
			return owns_lock();
		}
	protected:
	private:
		mutex_type * m_mutex;
		bool m_owns;
	};

	template<class Mutex>
	void swap(pie::unique_lock<Mutex> & left, pie::unique_lock<Mutex> & right)
	{
		left.swap(right);
	}
}