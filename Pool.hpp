#pragma once

#include <cassert>		// assert
#include <cstddef>		// std::size_t

#include <new>			// operator new[], operator delete[]
#include <memory>		// std::unique_ptr
#include <limits>		// std::numeric_limits

#include "Memory.hpp"

namespace cdp
{
	template <typename T>
	class HeapPool
	{
	public:
		using value_type = T;

		/// Default constructor
		HeapPool() = delete;
		/// Copy constructor
		HeapPool(const HeapPool& original) = delete;
		/// Move constructor
		HeapPool(HeapPool&& original) = default;
		/// Destructor
		~HeapPool() = default;

		/// Copy assignment operator
		HeapPool& operator = (const HeapPool& rhs) = delete;
		/// Move assignment operator
		HeapPool& operator = (HeapPool&& rhs) = default;

		/// Main constructor
		HeapPool(const std::size_t n) : buffer{ ::operator new[](sizeof(T)*n) }, pool_size{ n }, allocated{ 0 }
		{
			assert(0 < n && n <= std::numeric_limits<std::size_t>::max()/sizeof(T));
		}

		T* allocate(const std::size_t n);
		void deallocate(T* const p, std::size_t) const noexcept;

	private:
		std::unique_ptr<T[], cdp::operator_delete<T[]>>		buffer;
		const std::size_t									pool_size;
		std::size_t											allocated;
	};

	template <typename T, std::size_t N>
	class StackPool
	{
		static_assert(N > 0, "Some stack memory must be reserved");
		static_assert(N <= std::numeric_limits<std::size_t>::max()/sizeof(T), "The amonut of memory requested is too large");

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

		T* allocate(const std::size_t n);
		void deallocate(T* const p, std::size_t) const noexcept;

	private:
		union
		{
			byte			stack_buffer[sizeof(T)*N];
			T				buffer[];
		};
		std::size_t			allocated;
	};

	// DEFINITIONS

	template <typename T>
	T* HeapPool<T>::allocate(const std::size_t n)
	{
		assert(n > 0 && allocated + n <= pool_size);

		const auto index = allocated;
		allocated += n;

		return buffer + index;
	}

	template <typename T>
	void HeapPool<T>::deallocate(T* const p, std::size_t) const noexcept {}

	template <typename T, std::size_t N>
	T* StackPool<T>::allocate(const std::size_t n)
	{
		assert(n > 0 && allocated + n <= N);

		const auto index = allocated;
		allocated += n;

		return buffer + index;
	}
	
	template <typename T, std::size_t N>
	void StackPool<T>::deallocate(T* const p, std::size_t) const noexcept {}
}
