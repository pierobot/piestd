#pragma once

#include <pie/atomic.hpp>
#include <pie/cstddef.hpp>
#include <pie/type_traits.hpp>

namespace pie
{
#ifdef __EDG__
	namespace detail
	{
#pragma warning(push)
#pragma warning(disable: 4172)
	// Return address of function
		template<class T>
		inline T * addressof_helper(T & arg, pie::true_type) noexcept
		{
			return arg;
		}

		// Return address of object
		template<class T>
		inline T * addressof_helper(T& arg, pie::false_type)
		{
			return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(arg)));
		}
#pragma warning(pop)
	}
	
	template<class T>
	inline addressof(T & arg) noexcept
	{
		return pie::detail::addressof_helper(arg, pie::is_function<T>());
	}
#else	
	template<class T>
	inline T * addressof(T & arg) noexcept
	{
		return __builtin_addressof(arg);
	}
#endif

    template<class Pointer>
    struct pointer_traits
    {
        typedef Pointer pointer;
        typedef typename Pointer::element_type element_type;
        typedef typename Pointer::difference_type difference_type;

        static pointer pointer_to(element_type & element)
        {
            return addressof(element);
        }

		template<class U>
		using rebind = U*;
    };

    template<class T>
    struct pointer_traits<T*>
    {
        typedef T *pointer;
        typedef typename T element_type;
        typedef typename ptrdiff_t difference_type;

        static pointer pointer_to(element_type & element)
        {
            return addressof(element);
        }

		template<class U>
		using rebind = U*;
    };

	template<class Allocator>
	struct allocator_traits
	{
		typedef Allocator allocator_type;
		typedef typename Allocator::value_type value_type;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;
		typedef void * void_pointer;
		typedef void const * const_void_pointer;
		/*typedef typename pointer_traits<pointer>::rebind<void> void_pointer;
		typedef typename pointer_traits<pointer>::rebind<const void> const_void_pointer;*/
		typedef typename Allocator::difference_type difference_type;
		typedef typename Allocator::size_type size_type;

		//template<class U>
		//using rebind_alloc = Allocator<U>;

		//template<class U>
		//using rebind_traits = pie::allocator_traits<rebind_alloc<U>>;

		static pointer allocate(allocator_type & alloc, size_type size)
		{
			return alloc.allocate(size);
		}

		static pointer allocate(allocator_type & alloc, size_type size, const_void_pointer hint)
		{
			return alloc.allocate(size);
		}

		static void deallocate(allocator_type & alloc, pointer ptr, size_type size)
		{
			alloc.deallocate(ptr, size);
		}

		template<class T, class... Args>
		static void construct(allocator_type & alloc, T * ptr, Args &&... args)
		{
			alloc.construct(ptr, pie::forward<Args>(args)...);
		}

		template<class T>
		static void destroy(allocator_type & alloc, T * ptr)
		{
			alloc.destroy(ptr);
		}

		static size_type max_size(allocator_type & alloc) noexcept
		{
			return alloc.max_size();
		}
	};

    template<class T>
    struct default_delete
    {
        default_delete() noexcept = default;
        
        template<class U, class = typename pie::enable_if<pie::is_convertible<U*, T*>::value>::type>
		default_delete(default_delete<U> const & other)
		{
		}

        void operator()(T * ptr) const
        {
			if (ptr != nullptr)
				delete ptr;
        }
    };

	template<class T>
	struct default_delete<T[]>
	{
		default_delete() noexcept = default;

		template<class U, class = typename pie::enable_if<pie::is_convertible<U(*)[], T(*)[]>::value>::type>
		default_delete(default_delete<U[]> const & other) noexcept
		{
		}

		void operator()(T * ptr) const
		{
			if (ptr != nullptr)
				delete[] ptr;
		}
	};

	template<class Type, class Deleter = pie::default_delete<Type>>
	class unique_ptr
	{
	public:
		typedef Type * pointer;
		typedef Type element_type;
		typedef Deleter deleter_type;

		unique_ptr() noexcept = default;
		
		unique_ptr(pie::nullptr_t) noexcept :
			m_ptr(nullptr),
			m_deleter()
		{
		}
		
		explicit unique_ptr(pointer ptr) noexcept : 
			m_ptr(ptr),
			m_deleter()
		{
		}

		unique_ptr(pointer ptr, typename pie::remove_reference<Deleter>::type const del) noexcept :
			m_ptr(ptr),
			m_deleter(del)
		{
		}

		unique_ptr(pointer ptr, typename pie::remove_reference<Deleter>::type && del) noexcept :
			m_ptr(ptr),
			m_deleter(del)
		{
		}
		
		unique_ptr(unique_ptr && other) noexcept :
			m_ptr(other.release()),
			m_deleter(pie::move(other.get_deleter()))
		{
		}

		template<class OtherType, class OtherDeleter,
				 class = pie::enable_if_t<pie::is_convertible<typename pie::unique_ptr<OtherType, OtherDeleter>::pointer, pointer>::value &&
				  					     !pie::is_array<OtherType>::value &&     
				 					      pie::is_nothrow_move_constructible<OtherDeleter>::value>>
		unique_ptr(unique_ptr<OtherType, OtherDeleter> && other) noexcept :
			m_ptr(other.release()),
			m_deleter(pie::move(other.get_deleter()))
		{
		}

		template<class OtherType,
				 class = pie::enable_if_t<pie::is_convertible<typename pie::unique_ptr<OtherType>::pointer, Type>::value &&
				                          pie::is_same<typename pie::unique_ptr<OtherType>::deleter_type, deleter_type>::value>>
		unique_ptr(unique_ptr<OtherType> && other) noexcept :
			m_ptr(),
			m_deleter()
		{
			pie::swap(m_ptr, other.m_ptr);
			pie::swap(m_deleter, other.m_deleter);
		}

		~unique_ptr() noexcept
		{
			if (get() != pointer())
				get_deleter()(get());
		}

		pointer get() const noexcept
		{
			return m_ptr;
		}

		deleter_type & get_deleter() noexcept
		{
			return m_deleter;
		}

		deleter_type const & get_deleter() const noexcept
		{
			return m_deleter;
		}

		pointer release() noexcept
		{
			pointer ptr = get();
			m_ptr = nullptr;

			return ptr;
		}

		void reset(pointer ptr = pointer())
		{
			pointer old_ptr = m_ptr;
			m_ptr = ptr;

			if (old_ptr != nullptr)
				get_deleter()(old_ptr);
		}

		void swap(unique_ptr & other)
		{
			pie::swap(m_ptr, other.m_ptr);
		}

		explicit operator bool() const noexcept
		{
			return get() != nullptr;
		}

		typename pie::add_lvalue_reference<Type>::type operator*() const
		{
			return *get();
		}

		pointer operator->() const noexcept
		{
			return get();
		}
		
		bool operator==(pie::nullptr_t) const noexcept
		{
			return m_ptr == nullptr;
		}

		bool operator!=(pie::nullptr_t) const noexcept
		{
			return !(*this == nullptr);
		}
	protected:
	private:
		pointer m_ptr;
		deleter_type m_deleter;
	};

	template <class T, class D>
	bool operator!=(pie::nullptr_t, pie::unique_ptr<T, D> const & x) noexcept
	{
		return x.get() != nullptr;
	}

	template <class T, class D>
	bool operator!=(pie::unique_ptr<T, D> const & x, pie::nullptr_t) noexcept
	{
		return x.get() != nullptr;
	}

	template<class Type, class Deleter>
	class unique_ptr<Type[], Deleter>
	{
	public:
		typedef Type * pointer;
		typedef Type element_type;
		typedef Deleter deleter_type;

		unique_ptr() noexcept = default;

		unique_ptr(pie::nullptr_t) noexcept :
			m_ptr(nullptr),
			m_deleter()
		{
		}

		explicit unique_ptr(pointer ptr) noexcept :
			m_ptr(ptr),
			m_deleter()
		{
		}

		unique_ptr(pointer ptr, typename pie::remove_reference<Deleter>::type const del) noexcept :
			m_ptr(ptr),
			m_deleter(del)
		{
		}

		unique_ptr(pointer ptr, typename pie::remove_reference<Deleter>::type && del) noexcept :
			m_ptr(ptr),
			m_deleter(del)
		{
		}

		unique_ptr(unique_ptr && other) noexcept :
			m_ptr(other.release()),
			m_deleter(pie::move(other.get_deleter()))
		{
		}

		template<class OtherType, class OtherDeleter,
			class = pie::enable_if<pie::is_convertible<typename pie::unique_ptr<OtherType, OtherDeleter>::pointer, pointer>::value &&
			!pie::is_array<OtherType>::value &&
			pie::is_nothrow_move_constructible<OtherDeleter>::value>::type>
		unique_ptr(unique_ptr<OtherType, OtherDeleter> && other) noexcept :
			m_ptr(other.release()),
			m_deleter(pie::move(other.get_deleter()))
		{
		}

		template<class OtherType,
			class = pie::enable_if<pie::is_convertible<typename pie::unique_ptr<OtherType>::pointer, Type>::value &&
			pie::is_same<typename pie::unique_ptr<OtherType>::deleter_type, deleter_type>::value>::type>
		unique_ptr(unique_ptr<OtherType> && other) noexcept :
			m_ptr(),
			m_deleter()
		{
			pie::swap(m_ptr, other.m_ptr);
			pie::swap(m_deleter, other.m_deleter);
		}

		~unique_ptr() noexcept
		{
			if (get() != pointer())
				get_deleter()(get());
		}

		unique_ptr & operator=(unique_ptr && other)
		{
			m_ptr = other.m_ptr;
			other.m_ptr = nullptr;

			return *this;
		}

		template<class OtherType, class OtherDeleter>
		unique_ptr & operator=(unique_ptr<OtherType, OtherDeleter> && other)
		{
			static_assert(pie::is_convertible<typename pie::unique_ptr<OtherType, OtherDeleter>::pointer, pointer>::value, "OtherType must be convertible to Type");
			static_assert(!pie::is_array<OtherType>::value, "OtherType must not be an array.");
			static_assert(pie::is_nothrow_move_constructible<OtherDeleter>::value, "OtherDeleter must be nothrow move constructible.");

			m_ptr = other.m_ptr;
			m_deleter = pie::move(other.m_deleter);
			other.m_ptr = nullptr;

			return *this;
		}

		unique_ptr & operator=(pie::nullptr_t)
		{
			m_ptr = nullptr;

			return *this;
		}

		pointer get() const noexcept
		{
			return m_ptr;
		}

		deleter_type & get_deleter() noexcept
		{
			return m_deleter;
		}

		deleter_type const & get_deleter() const noexcept
		{
			return m_deleter;
		}

		void reset(pointer ptr = pointer())
		{
			pointer old_ptr = m_ptr;
			m_ptr = ptr;

			if (old_ptr != nullptr)
				get_deleter()(old_ptr);
		}

		template<class U>
		void reset(U) = delete;

		pointer release() noexcept
		{
			pointer ptr = get();
			m_ptr = nullptr;

			return ptr;
		}

		void swap(unique_ptr & other)
		{
			pie::swap(m_ptr, other.m_ptr);
			pie::swap(m_ptr, other.m_ptr);
		}

		explicit operator bool() const noexcept
		{
			return get() != nullptr;
		}

		typename pie::add_lvalue_reference<Type>::type operator[](int index) const
		{
			return get()[index];
		}

		bool operator==(pie::nullptr_t) const noexcept
		{
			return m_ptr == nullptr;
		}

		bool operator!=(pie::nullptr_t) const noexcept
		{
			return !(*this == nullptr);
		}
	protected:
	private:
		pointer m_ptr;
		deleter_type m_deleter;
	};

	namespace detail
	{
		template<class T>
		class ref_count
		{
		public:
			ref_count() noexcept :
				m_use_count(1)
			{
			}

			virtual ~ref_count()
			{
			}

			virtual void * get_deleter() noexcept
			{
				return nullptr;
			}

			virtual void * get_allocator() noexcept
			{
				return nullptr;
			}

			void increment() noexcept
			{
				++m_use_count;
			}

			void decrement() noexcept
			{
				--m_use_count;
			}

			long get_count() noexcept
			{
				return m_use_count.load(pie::memory_order::memory_order_relaxed);
			}

			bool is_only_reference() noexcept
			{
				return m_use_count.fetch_add(0, pie::memory_order::memory_order_relaxed) == 1;
			}

			void reset() noexcept
			{
				m_use_count.store(0, pie::memory_order::memory_order_relaxed);
			}

			virtual void destroy(T * ptr) noexcept
			{
				if (ptr != nullptr)
					delete ptr;
			}
		protected:
		private:
			pie::atomic_long m_use_count;
		};

		template<class T, class Deleter>
		class ref_count_deleter : public ref_count<T>
		{
		public:
			ref_count_deleter(Deleter deleter) noexcept :
				ref_count(),
				m_deleter(deleter)
			{
			}

			virtual ~ref_count_deleter()
			{
			}

			virtual void * get_deleter() noexcept override
			{
				return reinterpret_cast<void*>(pie::addressof(m_deleter));
			}

			virtual void destroy(T * ptr) override
			{
				m_deleter(ptr);
				
			}
		protected:
			Deleter m_deleter;
		private:
		};

		template<class T, class Deleter, class Allocator>
		class ref_count_deleter_allocator : public ref_count_deleter<T, Deleter>
		{
		public:
			ref_count_deleter_allocator(Deleter deleter, Allocator alloc) noexcept :
				ref_count_deleter(deleter),
				m_allocator(alloc)
			{
			}

			virtual ~ref_count_deleter_allocator()
			{
			}

			virtual void * get_allocator() noexcept override
			{
				return reinterpret_cast<void *>(pie::addressof(m_allocator));
			}
		protected:
		private:
			Allocator m_allocator;
		};

		template<class T>
		class basic_shared_ptr
		{
		public:
			typedef T element_type;

			basic_shared_ptr(element_type * ptr = nullptr) :
				m_ptr(ptr),
				m_ref_count(new detail::ref_count<element_type>())
			{
			}

			template<class Deleter>
			basic_shared_ptr(element_type * ptr, Deleter deleter) :
				m_ptr(ptr),
				m_ref_count(new detail::ref_count_deleter<element_type>(deleter))
			{
			}

			basic_shared_ptr(detail::ref_count<element_type> * ref_count) :
				m_ptr(),
				m_ref_count(ref_count)
			{
			}

			~basic_shared_ptr()
			{
				destroy();
			}

			element_type * get() const noexcept
			{
				return m_ptr;
			}

			detail::ref_count<element_type> * get_ref_count() const noexcept
			{
				return m_ref_count;
			}
		protected:
			element_type * m_ptr;
			detail::ref_count<element_type> * m_ref_count;

			void destroy()
			{
				if (m_ref_count == nullptr)
					return;

				m_ref_count->decrement();
				if (m_ref_count->get_count() != 0)
					return;

				m_ref_count->destroy(m_ptr);
				delete m_ref_count;

				m_ptr = nullptr;
				m_ref_count = nullptr;
			}
		private:
		};
	}
	
	template<class T>
	class shared_ptr : public detail::basic_shared_ptr<T>
	{
	public:
		shared_ptr() noexcept :
			basic_shared_ptr()
		{
		}

		shared_ptr(pie::nullptr_t) noexcept :
			basic_shared_ptr()
		{
		}

		template<class U>
		explicit shared_ptr(U * ptr) : // can throw bad_alloc
			basic_shared_ptr(ptr)
		{
		}

		template<class U, class Deleter>
		explicit shared_ptr(U * ptr, Deleter deleter) :// can throw bad_alloc
			basic_shared_ptr(ptr, deleter)
		{
			static_assert(pie::is_convertible<U*, T*>::value, "Type 'U' is not implicitly convertible to type 'T'.");
			static_assert(pie::is_copy_constructible<Deleter>::value, "Type 'Deleter' is not copy constructible.");
		}

		template<class Deleter>
		shared_ptr(pie::nullptr_t, Deleter deleter) :
			basic_shared_ptr(nullptr, deleter)
		{
		}

		template<class U>
		shared_ptr(shared_ptr<U> & other, element_type * ptr) noexcept
		{
			static_assert(pie::is_convertible<U*, T*>::value, "Type 'U' is not implicitly convertible to type 'T'.");
		}

		shared_ptr(shared_ptr const & other) noexcept :
			basic_shared_ptr()
		{
			assign(other);
		}

		template<class U>
		shared_ptr(shared_ptr<U> const & other) noexcept
		{
			static_assert(pie::is_convertible<U*, T*>::value, "Type 'U' is not implicitly convertible to type 'T'.");
			assign(other);
		}

		shared_ptr(shared_ptr && other) noexcept
		{
			swap_rvalue(pie::move(other));
		}

		template<class U>
		shared_ptr(shared_ptr<U> && other) noexcept
		{
			static_assert(pie::is_convertible<U*, T*>::value, "Type 'U' is not implicitly convertible to type 'T'.");

			swap_rvalue(pie::move(other));
		}

		template<class U, class Deleter>
		shared_ptr(pie::unique_ptr<U, Deleter> && other)
		{
			static_assert(pie::is_convertible<U*, T*>::value, "Type 'U' is not implicitly convertible to type 'T'.");
			static_assert(pie::is_copy_constructible<Deleter>::value, "Type 'Deleter' is not copy constructible.");
		}

		~shared_ptr()
		{
		}

		shared_ptr & operator=(shared_ptr const & other) noexcept
		{
			assign(other);

			return *this;
		}

		template<class U>
		shared_ptr & operator=(shared_ptr<U> const & other) noexcept
		{
			static_assert(pie::is_convertible<U*, T*>::value, "Type 'U' is not implicitly convertible to type 'T'.");

			assign(other);

			return *this;
		}

		shared_ptr & operator=(shared_ptr && other)
		{
			assign(pie::move(other));

			return *this;
		}

		template<class U>
		shared_ptr & operator=(shared_ptr<U> && other)
		{
			static_assert(pie::is_convertible<U*, T*>::value, "Type 'U' is not implicitly convertible to type 'T'.");

			assign(pie::move(other));

			return *this;
		}

		template<class U, class Deleter>
		shared_ptr & operator=(pie::unique_ptr<U, Deleter> && other)
		{
			static_assert(pie::is_convertible<U*, T*>::value, "Type 'U' is not implicitly convertible to type 'T'.");
			static_assert(pie::is_copy_constructible<Deleter>::value, "Type 'Deleter' is not copy constructible.");

			return *this;
		}

		void reset() noexcept
		{
			shared_ptr().swap(*this);
		}

		template<class U>
		void reset(U * ptr)
		{
			static_assert(pie::is_convertible<U*, T*>::value, "Type 'U' is not implicitly convertible to type 'T'.");

			// can throw bad_alloc
			shared_ptr<element_type>(ptr).swap(*this);

			if (m_ptr == nullptr)
				delete ptr;
		}

		template<class U, class Deleter>
		void reset(U * ptr, Deleter deleter)
		{
			static_assert(pie::is_convertible<U*, T*>::value, "Type 'U' is not implicitly convertible to type 'T'.");
			static_assert(pie::is_copy_constructible<Deleter>::value, "Type 'Deleter' is not copy constructible.");

			// can throw bad_alloc
			shared_ptr<element_type>(ptr, deleter).swap(*this);

			if (m_ptr == nullptr)
				deleter(ptr);
		}

		void swap(shared_ptr & other) noexcept
		{
			pie::swap(*this, other);
		}

		element_type & operator*() const noexcept
		{
			return *get();
		}

		element_type * operator->() const noexcept
		{
			return get();
		}

		long use_count() const noexcept
		{
			return m_ref_count;
		}

		bool unique() const noexcept
		{
			return use_count() == 1;
		}

		explicit operator bool() const noexcept
		{
			return get() != nullptr;
		}

		bool operator==(pie::nullptr_t) const noexcept
		{
			return get() == nullptr;
		}

		bool operator!=(pie::nullptr_t) const noexcept
		{
			return !(*this == nullptr);
		}
	protected:
	private:
		template<class U>
		void assign(shared_ptr<U> const & other) noexcept
		{
			if (m_ref_count == nullptr)
				return; // throw ?
			
			destroy();
			
			m_ptr = other.get();
			m_ref_count = other.get_ref_count();

			m_ref_count->increment();
		}

		template<class U>
		void assign(shared_ptr<U> && other) noexcept
		{
			if (m_ref_count != nullptr)
			{
				destroy();

				pie::swap(m_ptr, other.m_ptr);
				pie::swap(m_ref_count, other.m_ref_count);
			}

		}

		template<class U>
		void swap_rvalue(shared_ptr<U> && other)
		{
			shared_ptr<element_type> tmp;
			tmp.swap(other);

			swap(tmp);
		}

		bool is_empty() const noexcept
		{
			if (m_ref_count != nullptr)
			{
				if (m_ptr != nullptr)
					if (m_ref_count->load(pie::memory_order::memory_order_relaxed) != 0)
						return false;
			}

			return true;
		}
	};

	struct allocator_arg_t {};
}
