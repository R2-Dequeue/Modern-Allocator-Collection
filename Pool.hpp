#pragma once

#include <cassert>		// assert
#include <cstddef>		// std::size_t

#include <new>			// operator new[], operator delete[]
#include <memory>		// std::unique_ptr
#include <limits>		// std::numeric_limits

#include "Memory.hpp"	// cdp::operator_delete

/// My default namespace
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
		HeapPool(const std::size_t n);

		/// Allocates memory for \p n objects of type \p T and returns a pointer to the first
		T* allocate(const std::size_t n) noexcept;
		/// Deallocates memory previously allocated with a call to \code allocate \endcode
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
		static_assert(N <= std::numeric_limits<std::size_t>::max()/sizeof(T), "The amount of memory requested is too large");

	public:
		using value_type = T;
		using byte = unsigned char;

		/// Default constructor
		StackPool() noexcept;
		/// Copy constructor
		StackPool(const StackPool& original) = delete;
		/// Move constructor
		StackPool(StackPool&& original) = delete;
		/// Destructor
		~StackPool() = default;

		/// Copy assignment operator
		StackPool& operator = (const StackPool& rhs) = delete;
		/// Move assignment operator
		StackPool& operator = (StackPool&& rhs) = delete;

		T* allocate(const std::size_t n) noexcept;
		void deallocate(T* const p, std::size_t) const noexcept;

	private:
		union
		{
			byte		stack_buffer[sizeof(T)*N];
			T			buffer[0];
		};
		std::size_t		allocated;
	};

	// DEFINITIONS

	// HeapPool

	template <typename T>
	HeapPool<T>::HeapPool(const std::size_t n) : buffer{ ::operator new[](sizeof(T)*n) }, pool_size{ n }, allocated{ 0 }
	{
		assert(0 < n && n <= std::numeric_limits<std::size_t>::max() / sizeof(T));
	}

	template <typename T>
	T* HeapPool<T>::allocate(const std::size_t n) noexcept
	{
		assert(n > 0 && n + allocated <= pool_size);

		const auto index = allocated;
		allocated += n;

		return buffer + index;
	}

	template <typename T>
	void HeapPool<T>::deallocate(T* const p, std::size_t) const noexcept {}

	// StackPool

	template <typename T, std::size_t N>
	StackPool<T>::StackPool() noexcept : allocated{ 0 } {};

	template <typename T, std::size_t N>
	T* StackPool<T>::allocate(const std::size_t n) noexcept
	{
		assert(n > 0 && n + allocated <= N);

		const auto index = allocated;
		allocated += n;

		return buffer + index;
	}
	
	template <typename T, std::size_t N>
	void StackPool<T>::deallocate(T* const p, std::size_t) const noexcept {}
}
