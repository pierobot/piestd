#pragma once

#include <pie/memory.hpp>
#include <pie/mutex.hpp>
#include <pie/threads.h>

namespace pie
{
	enum class cv_status
	{
		no_timeout,
		timeout
	};

	class condition_variable
	{
	public:
		typedef void * native_handle_type;

		condition_variable() :
			m_mutex_ptr(new pie::mutex())
		{
			if (m_mutex_ptr == nullptr)
			{
				// throw bad_alloc
				return;
			}

			if (cnd_init(&m_cnd) == thrd_error)
			{
				// throw std::system_error;
			}
		}

		condition_variable(condition_variable const &) = delete;
		condition_variable & operator=(condition_variable const &) = delete;

		~condition_variable()
		{
			cnd_destroy(&m_cnd);
		}

		void notify_one() noexcept
		{
			m_mutex_ptr->lock();

			cnd_signal(&m_cnd);

			m_mutex_ptr->unlock();
		}

		void notify_all() noexcept
		{
			m_mutex_ptr->lock();

			cnd_broadcast(&m_cnd);

			m_mutex_ptr->unlock();
		}

		void wait(pie::unique_lock<pie::mutex> & lock)
		{
			pie::shared_ptr<pie::mutex> mutex_ptr = m_mutex_ptr;
			mutex_ptr->lock();
			lock.unlock();

			cnd_wait(&m_cnd, mutex_ptr->native_handle());
			
			mutex_ptr->unlock();
			lock.lock();
		}

		template<class Predicate>
		void wait(pie::unique_lock<pie::mutex> & lock, Predicate predicate)
		{
			while (!predicate())
				wait(lock);
		}

		/*template<class Rep, class Period>
		pie::cv_status wait_for(pie::unique_lock<pie::mutex> & lock, pie::chrono::duration<Rep, Period> const & rel_time)
		{
		}*/

		/*template<class Rep, class Period, class Predicate>
		bool wait_for(pie::unique_lock<pie::mutex> & lock, pie::chrono::duration<Rep, Period> const & rel_time, Predicate predicate)
		{
		}*/

		/*template<class Clock, class Duration>
		pie::cv_status wait_until(pie::unique_lock<pie::mutex> & lock, pie::chrono::time_point<Clock, Duration> & timeout_time)
		{
		}*/

		/*template<class Clock, class Duration, class Predicate>
		bool wait_until(pie::unique_lock<pie::mutex> & lock, pie::chrono::time_point<Clock, Duration> & timeout_time, Predicate predicate)
		{
		}*/

		native_handle_type native_handle() noexcept
		{
			return &m_cnd;
		}
	protected:
	private:
		pie::shared_ptr<pie::mutex> m_mutex_ptr;
		cnd_t m_cnd;
	};
}
