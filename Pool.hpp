#pragma once

#include <cstddef>		// std::size_t

#include <cassert>		// assert

#include <new>			// operator new[], operator delete[]
#include <memory>		// std::unique_ptr

namespace cdp
{
	template <typename T>
	class Pool
	{
	public:
		using value_type = T;

		/// Default constructor
		Pool() = delete;
		/// Copy constructor
		Pool(const Pool& original) = delete;
		/// Move constructor
		Pool(Pool&& original) = default;
		/// Destructor
		~Pool() = default;

		/// Copy assignment operator
		Pool& operator = (const Pool& rhs) = delete;
		/// Move assignment operator
		Pool& operator = (Pool&& rhs) = default;

		/// Main constructor
		Pool(const std::size_t n) : buffer{ ::operator new[](sizeof(T)*n) }, pool_size{ n }, allocated{ 0 }
		{
			assert(n > 0);
		}

	private:
		// cdp::uninitialized_delete or cdp::operator_delete would be useful
		std::unique_ptr<T[], cdp::operator_delete<T[]>>		buffer;
		const std::size_t									pool_size;
		std::size_t											allocated;
	};

	template <typename T, std::size_t N>
	class StackPool
	{
		static_assert(N > 0, "");
		static_assert(N <= std::numeric_limits<std::size_t>::max()/sizeof(T), "");

	public:
		using value_type = T;
		using byte = unsigned char;

		/// Default constructor
		StackPool() : allocated{ 0 } {};
		/// Copy constructor
		StackPool(const StackPool& original) = delete;
		/// Move constructor
		StackPool(StackPool&& original) = default;
		/// Destructor
		~StackPool() = default;

		/// Copy assignment operator
		StackPool& operator = (const StackPool& rhs) = delete;
		/// Move assignment operator
		StackPool& operator = (StackPool&& rhs) = default;

		T& operator [] (const std::size_t i) noexcept
		{
			assert(0 <= i && i < N);

			return buffer[i];
		}

	private:
		union
		{
			byte			stack_buffer[sizeof(T)*N];
			T				buffer[];
		}
		std::size_t			allocated;
	};
}
