#pragma once

#include <pie/functional.hpp>
#include <pie/tuple.hpp>
#include <pie/threads.h>

#include <Windows.h>
#include <process.h>

namespace pie
{
    namespace detail
    {
        // http://stackoverflow.com/a/7858971
        template<int ...> struct seq {};

        template<int N, int ...S> struct gens : gens<N - 1, N - 1, S...> {};

        template<int ...S> struct gens<0, S...> { typedef seq<S...> type; };

        template <typename Ret, typename ...Args>
        struct Dispatcher
        {
            template<int ...S>
            Ret callFunc(seq<S...>)
            {
                return func_(pie::get<S>(params_) ...);
            }

            pie::tuple<Args...> params_;
            Ret(*func_)(Args...);
        };
                
        template <typename Ret, typename ...Args>
        Ret call(Ret(*func)(Args...), pie::tuple<Args...> t)
        {
            Dispatcher<Ret, Args...> disp = { t, func };
            return disp.callFunc(typename gens<sizeof...(Args)>::type());
        }

        template<class Function, class... Args>
        class thread
        {
        public:
            thread() noexcept = default;

            thread(thread const &) = delete;

            thread(thread && other) noexcept :
                m_function(pie::move(other.m_function)),
                m_args(pie::forward<Args>(other.m_args)...)
            {

            }

            thread(Function function, Args &&... args) : 
				m_function(pie::forward<Function>(function)),
				m_args(pie::forward<Args>(args)...)
            {
				DWORD thread_id = 0;

				m_handle = ::CreateThread(nullptr, 0, &thread_function, this, 0, &thread_id);
				
            }

            void join()
			{
				if (m_handle != nullptr)
					::WaitForSingleObject(m_handle, INFINITE);
			}

			void * get_native_handle() const noexcept
			{
				return m_handle;
			}
        protected:
        private:
            static DWORD WINAPI thread_function(void * parameter) noexcept
            {
                auto thread_ptr = static_cast<thread *>(parameter);

                if (thread_ptr != nullptr)
                {
					thread_ptr->m_function(pie::forward<Args>(thread_ptr->m_args)...);
                }

                return 0;
            }

            Function m_function;
            pie::tuple<Args...> m_args;
			void * m_handle;
        };

		namespace this_thread
		{
			template<class Id>
			Id get_id() noexcept
			{
				return Id(::GetCurrentThread());
			}
		}
    }
}