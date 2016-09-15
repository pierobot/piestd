#pragma once

#include <pie/cstddef.hpp>
#include <pie/memory.hpp>
#include <pie/type_traits.hpp>

namespace pie
{
    template<class T>
    class reference_wrapper;
	
	template <class T>
	struct is_reference_wrapper : pie::false_type {};
	template <class U>
	struct is_reference_wrapper<pie::reference_wrapper<U>> : pie::true_type {};
	template <class T>
	bool is_reference_wrapper_v = is_reference_wrapper<T>::value;
	
    template<class T>
    class reference_wrapper
    {
    public:
        typedef T type;

        reference_wrapper(type & other) noexcept : m_object(pie::addressof(other)) {}
        reference_wrapper(type && other) = delete;
        reference_wrapper(reference_wrapper const & other) noexcept = default;

        reference_wrapper& operator=(reference_wrapper const & other) noexcept = default;

        operator T&() const noexcept { return *m_object; }
        T& get() const noexcept { return *m_object; }

        template<class... ArgTypes>
        auto operator()(ArgTypes &&... args) const -> decltype(pie::invoke(get(), pie::forward<ArgTypes>(args)...))
        {
            return pie::invoke(get(), pie::forward<ArgTypes>(args)...);
        }
    protected:
    private:
        T * m_object;
    };

    template<class T>
    auto ref(T & t) noexcept -> pie::reference_wrapper<T>
    { 
        return pie::reference_wrapper<T>(t);
    }

    template<class T>
    auto ref(pie::reference_wrapper<T> t) noexcept -> pie::reference_wrapper<T>
    {
        return ref(t.get());
    }

    template<class T>
    void ref(const T&&) = delete;

    template<class T>
    auto cref(T const & t) noexcept -> pie::reference_wrapper<T const>
    {
        return pie::reference_wrapper<T const>(t);
    }

    template<class T>
    auto cref(pie::reference_wrapper<T const> t) noexcept -> pie::reference_wrapper<T const>
    {
        return cref(t.get());
    }

    template<class T>
    void cref(const T&&) = delete;

	template<class MemPointer>
	class mem_fn_t
	{
	public:
		typedef MemPointer pointer;

		explicit mem_fn_t(pointer ptr) : m_ptr(ptr) { }

		template<class... ArgTypes>
		auto operator()(ArgTypes &&... args) const -> decltype(pie::invoke(m_ptr, pie::forward<ArgTypes>(args)...))
		{
			return pie::invoke(m_ptr, pie::forward<ArgTypes>(args)...);
		}
	protected:
	private:
		pointer m_ptr;
	};

	template<class Result, class Type>
	inline mem_fn_t<Result Type::*> mem_fn(Result Type::* ptr) noexcept
	{
		return mem_fn_t<Result Type::*>(ptr);
	}

	template <typename T>
	class function;

	// http://stackoverflow.com/a/38478032
	template <class Result, class... Args>
	class function<Result(Args...)>
	{
	public:
		typedef Result(*invoker_type)(void *, Args &&...);
		typedef void(*construct_copy_type)(void *, void *);
		typedef void(*construct_move_type)(void *, void  *);
		typedef void(*destroy_type)(void *);

		function() :
			m_data(),
			m_size(),
			m_invoker(),
			m_construct_copy(),
			m_construct_move(),
			m_destroy()
		{
		}

		function(function const & other) :
			m_data(),
			m_size(other.m_size),
			m_invoker(other.m_invoker),
			m_construct_copy(other.m_construct_copy),
			m_construct_move(other.m_construct_move),
			m_destroy(other.m_destroy)
		{
			reallocate(m_size);

			if (m_data != nullptr)
				m_construct_move(m_data.get(), other.m_data.get());
		}

		template<class F>
		function(F fn) :
			m_data(),
			m_size(),
			m_invoker(reinterpret_cast<invoker_type>(invoker<F>)),
			m_construct_copy(reinterpret_cast<construct_copy_type>(construct_copy<F>)),
			m_construct_move(reinterpret_cast<construct_move_type>(construct_move<F>)),
			m_destroy(reinterpret_cast<destroy_type>(destroy<F>))
		{

			reallocate(sizeof(fn) + sizeof(void*));

			if (m_data != nullptr)
				m_construct_copy(m_data.get(), pie::addressof(fn));
		}

		~function()
		{
			if (m_data != nullptr)
				m_destroy(m_data.get());
		}

		function & operator=(function const & other)
		{
			function(other).swap(*this);

			return *this;
		}

		function & operator=(function && other)
		{
			m_data = pie::move(other.m_data);
			m_size = other.m_size;
			m_invoker = other.m_invoker;
			m_construct_copy = other.m_construct_copy;
			m_construct_move = other.m_construct_move;
			m_destroy = other.m_destroy;

			other.m_size = 0;
			other.m_invoker = nullptr;
			other.m_construct_copy = nullptr;
			other.m_construct_move = nullptr;
			other.m_destroy = nullptr;

			return *this;
		}

		Result operator()(Args &&... args)
		{
			return m_invoker(m_data.get(), pie::forward<Args>(args)...);
		}

		void swap(function & other) noexcept
		{
			auto tmp = m_data.release();
			m_data.reset(other.m_data.release());
			other.m_data.reset(tmp);

			pie::swap(m_size, other.m_size);
			pie::swap(m_invoker, other.m_invoker);
			pie::swap(m_construct_copy, other.m_construct_copy);
			pie::swap(m_construct_move, other.m_construct_move);
			pie::swap(m_destroy, other.m_destroy);
		}

		bool operator==(pie::nullptr_t) const noexcept
		{
			return m_data == nullptr;
		}

		bool operator!=(pie::nullptr_t) const noexcept
		{
			return !(*this == nullptr);
		}
	protected:
	private:
		pie::unique_ptr<char[]> m_data;
		pie::size_t m_size;

		invoker_type m_invoker;
		construct_copy_type m_construct_copy;
		construct_move_type m_construct_move;
		destroy_type m_destroy;

		void reallocate(pie::size_t size)
		{
			m_data.reset(new char[size]);
			m_size = size;
		}

		template<class T>
		static Result invoker(T * fn, Args &&... args)
		{
			return pie::invoke(pie::forward<T>(*fn), pie::forward<Args>(args)...);
		}

		template<class T>
		static void construct_copy(T * dest, T * src)
		{
			new (dest) T(*src);
		}

		template<class T>
		static void construct_move(T * dest, T * src)
		{
			new (dest) T(pie::move(*src));
		}

		template<class T>
		static void destroy(T * obj)
		{
			obj->~T();
		}
	};

	template<class T>
	struct less
	{
		bool operator()(T const & left, T const & right) const noexcept
		{
			return left < right;
		}
	};
	
}
