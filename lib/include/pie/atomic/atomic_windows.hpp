#pragma once

#include <pie/stdint.hpp>
#include <pie/type_traits.hpp>
#include <intrin.h>

namespace pie
{
	enum memory_order
	{
		memory_order_relaxed,
		memory_order_consume,
		memory_order_acquire,
		memory_order_release,
		memory_order_acq_rel,
		memory_order_seq_cst
	};

	namespace detail
	{
		namespace relaxed
		{
			template<class T>
			inline void store(volatile T * ptr, T value)
			{
				*ptr = value;
			}

			template<class T>
			inline T load(volatile T * ptr)
			{
				return *ptr;
			}
		}

		namespace cst
		{
			template<unsigned> struct helper {};

			template<> struct helper<1>
			{
				static inline void store(volatile char * ptr, char value) noexcept
				{
					_InterlockedExchange8(ptr, value);
				}

				static inline char load(volatile char *ptr) noexcept
				{
					return _InterlockedOr8(ptr, 0);
				}
			};

			template<> struct helper<2>
			{
				static inline void store(volatile short * ptr, short value)
				{
					_InterlockedExchange16(ptr, value);
				}

				static inline short load(volatile short * ptr) noexcept
				{
					return _InterlockedOr16(ptr, 0);
				}
			};

			template<> struct helper<4>
			{
				static inline void store(volatile long * ptr, long value) noexcept
				{
					_InterlockedExchange(ptr, value);
				}

				static inline long load(volatile long * ptr) noexcept
				{
					return _InterlockedOr(ptr, 0);
				}
			};

			template<> struct helper<8>
			{
				static inline void store(volatile long long * ptr, long long value) noexcept
				{
#ifdef _AMD64_
					_InterlockedExchange64(ptr, value);
#endif
				}

				static inline long long load(volatile long long * ptr) noexcept
				{
#ifdef _AMD64_
					return _InterlockedOr64(ptr, 0);
#endif
				}
			};

			//inline void store(volatile char * ptr, char value)
			//{
			//	_InterlockedExchange8(ptr, value);
			//}

			//inline void store(volatile short * ptr, short value)
			//{
			//	_InterlockedExchange16(ptr, value);
			//}

			//inline void store(volatile long * ptr, long value)
			//{
			//	_InterlockedExchange(ptr, value);
			//}

			//inline void store(volatile long long * ptr, long long value)
			//{
			//	_InterlockedExchange64(ptr, value);
			//}

			//inline char load(volatile char * ptr)
			//{
			//	return _InterlockedOr8(ptr, 0);
			//}

			//inline short load(volatile short * ptr)
			//{
			//	return _InterlockedOr16(ptr, 0);
			//}

			//inline long load(volatile long * ptr)
			//{
			//	return _InterlockedOr(ptr, 0);
			//}

			//inline long long load(volatile long long * ptr)
			//{
			//	return _InterlockedOr64(ptr, 0);
			//}
		}

		template<class T>
		class basic_atomic
		{
		public:
			basic_atomic() noexcept = default;
			basic_atomic(T desired) noexcept : m_value(desired)
			{
			}

			basic_atomic(basic_atomic const &) = delete;

			basic_atomic & operator=(T value) noexcept
			{
				store(value);
				return *this;
			}

			/*basic_atomic & operator=(T value) volatile noexcept
			{
				store(value);
				return *this;
			}*/

			basic_atomic & operator=(basic_atomic const &) = delete;
			/*basic_atomic & operator=(basic_atomic const &) volatile = delete;*/

			void store(T value, memory_order order = pie::memory_order_seq_cst) noexcept
			{
				switch (order)
				{
				case pie::memory_order_relaxed:
				{
					relaxed::store(&m_value, value);
					break;
				}

				case pie::memory_order_seq_cst:
				{
					cst::helper<sizeof(T)>::store(&m_value, value);
					break;
				}

				default:
					break;
				}
			}

			void store(T value, memory_order order = pie::memory_order_seq_cst) volatile noexcept
			{
				switch (order)
				{
				case pie::memory_order_relaxed:
				{
					relaxed::store(&m_value, value);
					break;
				}

				case pie::memory_order_seq_cst:
				{
					cst::helper<sizeof(T)>::store(&m_value, value);
					break;
				}

				default:
					break;
				}
			}

			T load(memory_order order = pie::memory_order_seq_cst) noexcept
			{
				switch (order)
				{
				case pie::memory_order_relaxed:
				{
					return relaxed::load(&m_value);
				}

				case pie::memory_order_seq_cst:
				{
					return cst::helper<sizeof(T)>::load(&m_value);
				}

				default:
					break;
				}

				return T();
			}

			T load(memory_order order = pie::memory_order_seq_cst) volatile noexcept
			{
				switch (order)
				{
				case pie::memory_order_relaxed:
				{
					return relaxed::load(&m_value);
				}

				case pie::memory_order_seq_cst:
				{
					return cst::helper<sizeof(T)>::load(&m_value);
				}

				default:
					break;
				}

				return T();
			}

			T exchange(T value, memory_order = order = pie::memory_order_seq_cst) noexcept
			{
				T old_value = load(order);
				store(value, order);

				return old_value;
			}

			T exchange(T value, memory_order = order = pie::memory_order_seq_cst) volatile noexcept
			{
				T old_value = load(order);
				store(value, order);

				return old_value;
			}

			T fetch_add(T value, memory_order order = pie::memory_order_seq_cst) noexcept
			{
				T old_value = load(order);
				T new_value = old_value + value;
				store(new_value, order);

				return old_value;
			}

			T fetch_add(T value, memory_order order = pie::memory_order_seq_cst) volatile noexcept
			{
				T old_value = load(order);
				T new_value = old_value + value;
				store(new_value, order);

				return old_value;
			}

			T fetch_and(T value, memory_order order = pie::memory_order_seq_cst) noexcept
			{
				T old_value = load(order);
				T new_value = old_value & value;
				store(new_value, order);

				return old_value;
			}

			T fetch_and(T value, memory_order order = pie::memory_order_seq_cst) volatile noexcept
			{
				T old_value = load(order);
				T new_value = old_value & value;
				store(new_value, order);

				return old_value;
			}

			T fetch_sub(T value, memory_order order = pie::memory_order_seq_cst) noexcept
			{
				T old_value = load(order);
				T new_value = old_value - value;
				store(new_value, order);

				return old_value;
			}

			T fetch_sub(T value, memory_order order = pie::memory_order_seq_cst) volatile noexcept
			{
				T old_value = load(order);
				T new_value = old_value - value;
				store(new_value, order);

				return old_value;
			}

			T fetch_or(T value, memory_order order = pie::memory_order_seq_cst) noexcept
			{
				T old_value = load(order);
				T new_value = old_value | value;
				store(new_value, order);

				return old_value;
			}

			T fetch_or(T value, memory_order order = pie::memory_order_seq_cst) volatile noexcept
			{
				T old_value = load(order);
				T new_value = old_value | value;
				store(new_value, order);

				return old_value;
			}

			T fetch_xor(T value, memory_order order = pie::memory_order_seq_cst) noexcept
			{
				T old_value = load(order);
				T new_value = old_value ^ value;
				store(new_value, order);

				return old_value;
			}

			T fetch_xor(T value, memory_order order = pie::memory_order_seq_cst) volatile noexcept
			{
				T old_value = load(order);
				T new_value = old_value ^ value;
				store(new_value, order);

				return old_value;
			}

			bool is_lock_free() const noexcept
			{
				return true;
			}

			bool is_lock_free() const volatile noexcept
			{
				return true;
			}

			T operatorT() const noexcept
			{
				return load();
			}

			T operatorT() const volatile noexcept
			{
				return load();
			}

			T operator++() noexcept
			{
				return fetch_add(1) + 1;
			}

			T operator++() volatile noexcept
			{
				return fetch_add(1) + 1;
			}

			T operator++(int) noexcept
			{
				return fetch_add(1);
			}

			T operator++(int) volatile noexcept
			{
				return fetch_add(1);
			}

			T operator--() noexcept
			{
				return fetch_sub(1) + 1;
			}

			T operator--() volatile noexcept
			{
				return fetch_sub(1) + 1;
			}

			T operator--(int) noexcept
			{
				return fetch_sub(1);
			}

			T operator--(int) volatile noexcept
			{
				return fetch_sub(1);
			}

			T operator+=(T value) noexcept
			{
				return fetch_add(value) + value;
			}

			T operator+=(T value) volatile noexcept
			{
				return fetch_add(value) + value;
			}

			T operator-=(T value) noexcept
			{
				return fetch_sub(value) + value;
			}

			T operator-=(T value) volatile noexcept
			{
				return fetch_sub(value) + value;
			}

			T operator&=(T value) noexcept
			{
				return fetch_and(value) & value;
			}

			T operator&=(T value) volatile noexcept
			{
				return fetch_and(value) & value;
			}

			T operator|=(T value) noexcept
			{
				return fetch_or(value) | value;
			}

			T operator|=(T value) volatile noexcept
			{
				return fetch_or(value) | value;
			}

			T operator^=(T value) noexcept
			{
				return fetch_xor(value) ^ value;
			}

			T operator^=(T value) volatile noexcept
			{
				return fetch_xor(value) ^ value;
			}
		protected:
		private:
			T m_value;
		};
	}
}
