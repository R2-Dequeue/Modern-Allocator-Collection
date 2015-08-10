#pragma once

#include <cstddef>		// std::size_t

#include <new>			// std::bad_array_new_length
#include <limits>		// std::numeric_limits
#include <type_traits>	// std::is_const

#include "is_complete/type_traits.hpp"

/// My default namespace
namespace cdp
{
	// Allocator requirements: 17.6.3.5

	/// A basic reference C++11 allocator mostly for testing and documenting requirements from the standard
	template <typename T>
	class ReferenceAllocator
	{
		static_assert(cdp::is_complete<T>(), "Allocated type must be complete");
		static_assert(!std::is_const<T>::value, "Allocated type must be non-const (17.6.3.5/2)");

	public:
		using value_type = T;

		/// Default constructor
		ReferenceAllocator(...) noexcept {}
		/// Copy constructor
		ReferenceAllocator(const ReferenceAllocator& original) = default;
		/// Move constructor
		ReferenceAllocator(ReferenceAllocator&& original) = default;
		/// Destructor
		~ReferenceAllocator() = default;

		/// Copy assignment operator
		ReferenceAllocator& operator = (const ReferenceAllocator& rhs) = default;
		/// Move assignment operator
		ReferenceAllocator& operator = (ReferenceAllocator&& rhs) = default;

		/// Rebind constructor
		template <typename U>
		ReferenceAllocator(const ReferenceAllocator<U>& original) noexcept {}

		/// Allocates memory for \p n objects of type \p T and returns a pointer to the first
		T* allocate(const std::size_t n) const
		{
			if (n > std::numeric_limits<std::size_t>::max()/sizeof(T))
				throw std::bad_array_new_length{};
			
			// 18.6.1.1/3 and 18.6.1.2: returns properly-aligned pointer, throws std::bad_alloc on error
			return static_cast<T*>(::operator new[](sizeof(T)*n));
		}
		/// Deallocates memory previously allocated with a call to \code allocate \endcode
		void deallocate(T* const p, std::size_t) const noexcept
		{
			::operator delete[](p); // 18.6.1.2: doesn't throw
		}
	};

	template <typename T, typename U>
	inline bool operator == (const ReferenceAllocator<T>& lhs, const ReferenceAllocator<U>& rhs) noexcept
	{
		return true;
	}
	template <typename T, typename U>
	inline bool operator != (const ReferenceAllocator<T>& lhs, const ReferenceAllocator<U>& rhs) noexcept
	{
		return false;
	}
}