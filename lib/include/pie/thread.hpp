#pragma once

#include <pie/condition_variable.hpp>
#include <pie/functional.hpp>
#include <pie/mutex.hpp>
#include <pie/tuple.hpp>
#include <pie/type_traits.hpp>
#include <pie/utility.hpp>

namespace pie
{
	namespace detail
	{
		class basic_launcher
		{
		public:
			basic_launcher() : 
				m_started(false),
				m_conditional(),
				m_mutex()
			{
			}

			virtual ~basic_launcher()
			{
			}

			virtual void run() = 0;

			static int thread_function(void * this_ptr)
			{
				basic_launcher * launcher_ptr = static_cast<basic_launcher*>(this_ptr);
				launcher_ptr->run();

				return 0;
			}

			bool execute(thrd_t * thrd)
			{
				if (thrd != nullptr)
				{
					pie::unique_lock<pie::mutex> lock(m_mutex, pie::defer_lock_t());

					if (thrd_create(thrd, &thread_function, this) == thrd_success)
					{
						while (m_started == false)
							m_conditional.wait(lock);

						return true;
					}
				}

				return false;
			}

			void release()
			{
				m_mutex.lock();
				m_started = true;
				m_conditional.notify_one();
				m_mutex.unlock();
			}
		protected:
		private:
			bool m_started;
			pie::condition_variable m_conditional;
			pie::mutex m_mutex;
		};

		template<class Function, class TuplePtr>
		class launcher : public basic_launcher
		{
		public:
			template<class OtherTuplePtr>
			launcher(Function && fn, OtherTuplePtr && tup) :
				m_fn(pie::forward<Function>(fn)),
				m_tuple(pie::forward<OtherTuplePtr>(tup))
			{
			}

			virtual ~launcher()
			{
			}

			virtual void run() override final
			{
				execute1(this);
			}
		protected:
		private:
			Function m_fn;
			TuplePtr m_tuple;

			template<pie::size_t... Indexes>
			static void execute2(Function && fn, TuplePtr && tuple_ptr, pie::integer_sequence<pie::size_t,Indexes...>)
			{
				pie::invoke(pie::forward<Function>(fn), pie::move(pie::get<Indexes>(*tuple_ptr))...);
			}
			
			static void execute1(launcher * ptr)
			{
				TuplePtr tup(pie::forward<TuplePtr>(ptr->m_tuple));
				ptr->release();
				ptr->execute2(pie::forward<Function>(ptr->m_fn), pie::move(tup), pie::make_integer_sequence<pie::size_t, pie::tuple_size<typename TuplePtr::element_type>::value>());
			}
		};
	}

	class thread
    {
    public:
        typedef thrd_t * native_handle_type;
		
		class id
		{
		public:
			id() noexcept : m_thrd() {}
			explicit id(native_handle_type handle) : m_thrd(*handle) {}
		protected:
		private:
			friend class thread;
			friend bool operator==(thread::id, thread::id) noexcept;
			friend bool operator!=(thread::id, thread::id) noexcept;
			friend bool operator<(thread::id, thread::id) noexcept;
			friend bool operator<=(thread::id, thread::id) noexcept;
			friend bool operator>(thread::id, thread::id) noexcept;
			friend bool operator>=(thread::id, thread::id) noexcept;

			thrd_t m_thrd;
		};

        thread() noexcept = default;
        thread(thread const &) = delete;
        thread(thread&& other) noexcept
        {
            thread::swap(pie::move(other));
        }

        template<class Function, class... Args, typename = pie::enable_if<!pie::is_same<typename pie::decay<Function>::type, pie::thread>::value>::type>
        explicit thread(Function && function, Args &&... args)
        {
			typedef pie::tuple<typename pie::decay<Args>::type...> thread_tuple_type;
			
			pie::unique_ptr<thread_tuple_type> tuple_ptr(new thread_tuple_type(pie::decay_copy(pie::forward<Args>(args))...));
			
			if (tuple_ptr == nullptr)
			{
				// throw bad_alloc
				return;
			}

			detail::launcher<Function, decltype(tuple_ptr)> launcher(pie::forward<Function>(function), pie::move(tuple_ptr));
			launcher.execute(&m_id.m_thrd);
        }

        ~thread()
        {
        }

        void swap(thread && other) noexcept
        {
            pie::swap(m_id, other.m_id);
        }

        thread::id get_id() const noexcept
        {
            return m_id;
        }

        native_handle_type native_handle() noexcept
        {
            return &m_id.m_thrd;
        }

		void join();

		bool joinable() const noexcept
		{
			return get_id() != thread::id();
		}
    protected:
    private:
        thread::id m_id;
    };

    bool operator==(thread::id left, thread::id right) noexcept
    {
        return left.m_thrd.id == right.m_thrd.id;
    }

    bool operator!=(thread::id left, thread::id right) noexcept
    {
        return left.m_thrd.id != right.m_thrd.id;
    }

    bool operator<(thread::id left, thread::id right) noexcept
    {
        return left.m_thrd.id < right.m_thrd.id;
    }

    bool operator<=(thread::id left, thread::id right) noexcept
    {
        return left.m_thrd.id <= right.m_thrd.id;
    }

    bool operator>(thread::id left, thread::id right) noexcept
    {
        return left.m_thrd.id > right.m_thrd.id;
    }

    bool operator>=(thread::id left, thread::id right) noexcept
    {
        return left.m_thrd.id >= right.m_thrd.id;
    }

	namespace this_thread
	{
		thread::id get_id() noexcept
		{
			thrd_t thrd = thrd_current();
			return thread::id(&thrd);
		}
	}

	void thread::join() // throws system_error class
	{
		if (get_id() == pie::this_thread::get_id())
			; // throw resource_deadlock_would_occur

		if (joinable() == false)
			; // throw invalid_argument

		if (m_id == thread::id())
			; // throw no_such_process

		int result = 0;
		thrd_join(m_id.m_thrd, &result);
	}
}
