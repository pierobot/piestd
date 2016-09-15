#pragma once

#include <pie/type_traits/enable_if.hpp>
#include <pie/type_traits/integral_constant.hpp>

#include <pie/functional.hpp>
#include <pie/string.hpp>

namespace pie
{
	enum class errc
	{
		address_family_not_supported = 1,
		address_in_use,
		address_not_available,
		already_connected,
		argument_list_too_long,
		argument_out_of_domain,
		bad_address,
		bad_file_descriptor,
		bad_message,
		broken_pipe,
		connection_aborted,
		connection_already_in_progress,
		connection_refused,
		connection_reset,
		cross_device_link,
		destination_address_required,
		device_or_resource_busy,
		directory_not_empty,
		executable_format_error,
		file_exists,
		file_too_large,
		filename_too_long,
		function_not_supported,
		host_unreachable,
		identifier_removed,
		illegal_byte_sequence,
		inappropriate_io_control_operation,
		interrupted,
		invalid_argument,
		invalid_seek,
		io_error,
		is_a_directory,
		message_size,
		network_down,
		network_reset,
		network_unreachable,
		no_buffer_space,
		no_child_process,
		no_link,
		no_lock_available,
		no_message_available,
		no_message,
		no_protocol_option,
		no_space_on_device,
		no_stream_resources,
		no_such_device_or_address,
		no_such_device,
		no_such_file_or_directory,
		no_such_process,
		not_a_directory,
		not_a_socket,
		not_a_stream,
		not_connected,
		not_enough_memory,
		not_supported,
		operation_canceled,
		operation_in_progress,
		operation_not_permitted,
		operation_not_supported,
		operation_would_block,
		owner_dead,
		permission_denied,
		protocol_error,
		protocol_not_supported,
		read_only_file_system,
		resource_deadlock_would_occur,
		resource_unavailable_try_again,
		result_out_of_range,
		state_not_recoverable,
		stream_timeout,
		text_file_busy,
		timed_out,
		too_many_files_open_in_system,
		too_many_files_open,
		too_many_links,
		too_many_symbolic_link_levels,
		value_too_large,
		wrong_protocol_type
	};

	enum class io_errc
	{
		stream = 1
	};

	enum class future_errc
	{
	};

	template<class ErrorEnum>
	struct is_error_code_enum : false_type
	{
	};

	template<>
	struct is_error_code_enum<errc> : true_type
	{
	};

	template<>
	struct is_error_code_enum<io_errc> : true_type
	{
	};

	template<>
	struct is_error_code_enum<future_errc> : true_type
	{
	};

	template<class ErrorEnum>
	struct is_error_condition_enum : false_type
	{
	};

	template<>
	struct is_error_condition_enum<errc> : true_type
	{
	};

	class error_code;
	class error_condition;
	error_code make_error_code(errc ec) noexcept;
	error_code make_error_code(io_errc ec) noexcept;
	error_condition make_error_condition(errc ec) noexcept;
	error_condition make_error_condition(io_errc ec) noexcept;

	class error_category;
	error_category const & generic_category() noexcept;
	error_category const & system_category() noexcept;

	class error_category
	{
	public:
		error_category() = default;
		error_category(error_category const&) = delete;

		virtual ~error_category() {}

		error_category & operator=(error_category const&) = delete;

		virtual char const * name() const noexcept = 0;

		virtual error_condition default_error_condition(int code) const noexcept;

		virtual bool equivalent(int code, error_condition const & condition) const noexcept;
		virtual bool equivalent(error_code const & code, int condition) const noexcept;

#ifdef PIE_USE_ERRLIST
		virtual pie::string message(int condition) const = 0;
#endif

		bool operator==(error_category const & other) const noexcept
		{
			return this == &other;
		}

		bool operator!=(error_category const & other) const noexcept
		{
			return !(*this == other);
		}

		bool operator<(error_category const & other) const noexcept
		{
			return less<error_category const *>()(this, &other);
		}
	protected:
		enum : int
		{
			generic_condition_value = 1,
			io_stream_condition_value = 3,
			system_condition_value = 5,
			future_condition_value = 7
		};

		int m_value;
	private:
	};

	class error_code
	{
	public:
		error_code() noexcept :
			m_ec(),
			m_category()
		{
		}

		error_code(int ec, error_category const & category) noexcept :
		m_ec(ec),
			m_category(reinterpret_cast<error_category const*>(pie::addressof(category)))
		{
		}

		template<class ErrorCodeEnum, class = typename pie::enable_if<pie::is_error_code_enum<ErrorCodeEnum>::value>::type>
		error_code(ErrorCodeEnum e)
		{
			/*static_assert(pie::is_error_code_enum<ErrorCodeEnum>::value, "e must be a valid error_code enum class.");*/
			*this = e;
		}

		template<class ErrorCodeEnum, class = typename pie::enable_if<pie::is_error_code_enum<ErrorCodeEnum>::value>::type>
		error_code & operator=(ErrorCodeEnum e) noexcept
		{
			/*static_assert(pie::is_error_code_enum<ErrorCodeEnum>::value, "e must be a valid error_code enum class.");*/
			*this = pie::make_error_code(e);

			return *this;
		}

		void assign(int ec, error_category const & category) noexcept
		{
			m_ec = ec;
			m_category = reinterpret_cast<error_category const*>(pie::addressof(category));
		}

		void clear() noexcept
		{
			*this = error_code(0, system_category());
		}

		int value() const noexcept
		{
			return m_ec;
		}

		error_category const & category() const noexcept
		{
			return *m_category;
		}

		explicit operator bool() const noexcept
		{
			return m_ec == 0;
		}

	protected:
	private:
		int m_ec;
		error_category const * m_category;
	};

	bool operator==(error_code const & left, error_code const & right) noexcept
	{
		return left.value() == right.value() &&
			left.category() == right.category();
	}


	class error_condition
	{
	public:
		error_condition() noexcept :
			m_ec(),
			m_category()
		{
		}

		error_condition(error_condition const & other) noexcept
		{
		}

		error_condition(int value, error_category const & category) noexcept :
			m_ec(value),
			m_category(&category)
		{
		}

		template<class ErrorConditionEnum, class = typename pie::enable_if<pie::is_error_condition_enum<ErrorConditionEnum>::value>::type>
		error_condition(ErrorConditionEnum e) noexcept :
			m_ec(),
			m_category()
		{
			*this = make_error_condition(e);
		}

		error_condition & operator=(error_condition const & other) noexcept
		{
			m_ec = other.m_ec;
			m_category = other.m_category;

			return *this;
		}

		template<class ErrorConditionEnum, class = typename pie::enable_if<pie::is_error_condition_enum<ErrorConditionEnum>::value>::type>
		error_condition & operator=(ErrorConditionEnum e) noexcept
		{
			*this = make_error_condition(e);

			return *this;
		}

		void assign(int ec, error_category const & category) noexcept
		{
			m_ec = ec;
			m_category = &category;
		}

		void clear() noexcept
		{
			m_ec = 0;
			m_category = &generic_category();
		}

		int value() const noexcept
		{
			return m_ec;
		}

		error_category const & category() const noexcept
		{
			return *m_category;
		}

		explicit operator bool() const noexcept
		{
			return m_ec == 0;
		}
	protected:
	private:
		int m_ec;
		error_category const * m_category;
	};

	bool operator==(error_condition const & left, error_condition const & right) noexcept
	{
		return left.value() == right.value() &&
			   left.category() == right.category();
	}

	bool operator==(error_code const & left, error_condition const & right) noexcept
	{
		return left.category().equivalent(left.value(), right) ||
			   right.category().equivalent(left, right.value());
	}

	bool operator==(error_condition const & left, error_code const & right) noexcept
	{
		return right.category().equivalent(right.value(), left) ||
			   left.category().equivalent(right, left.value());
	}

	bool operator!=(error_condition const & left, error_condition const & right) noexcept
	{
		return !(left == right);
	}

	bool operator!=(error_code const & left, error_condition const & right) noexcept
	{
		return !(left == right);
	}

	bool operator!=(error_condition const & left, error_code const & right) noexcept
	{
		return !(left == right);
	}

	error_category const & generic_category() noexcept
	{
		class static_generic_category final : public error_category
		{
		public:
			static_generic_category() : error_category() {}
			virtual ~static_generic_category() {}

			virtual char const * name() const noexcept { return "generic"; }
#ifdef PIE_USE_ERRLIST
			virtual pie::string message(int condition) const
			{
				return "Unimplemented";
			}
#endif
		};

		static static_generic_category category;
		return category;
	}

	pie::error_category const & system_category() noexcept
	{
		class static_system_category final : public error_category
		{
		public:
			static_system_category() : error_category() {}
			virtual ~static_system_category() {}
			
			virtual char const * name() const noexcept { return "system"; }

#ifdef PIE_USE_ERRLIST
			virtual pie::string message(int condition) const
			{
				return "Unimplemented";
			}
#endif
		};

		static static_system_category category;
				
		return category;
	}

	error_code make_error_code(errc e) noexcept
	{
		return error_code(static_cast<int>(e), generic_category());
	}

	error_condition error_category::default_error_condition(int code) const noexcept
	{
		return error_condition(code, *this);
	}

	bool error_category::equivalent(int code, error_condition const & condition) const noexcept
	{
		return default_error_condition(code) == condition;
	}

	bool error_category::equivalent(error_code const & code, int condition) const noexcept
	{
		return *this == code.category() && code.value() == condition;
	}
}