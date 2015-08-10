#pragma once

#include <new>		// operator delete, operator delete[]

/// My default namespace
namespace cdp
{
	// Default deleters: 20.8.1.1

	/// A replacement for \code std::defaulte_delete \endcode that frees memory allocated with \code ::operator new \endcode
	template <typename T>
	class operator_delete
	{
	public:
		constexpr operator_delete() noexcept = default;

		template <typename U>
		operator_delete(const operator_delete<U>& other) noexcept {}

		void operator () (T* const p) const noexcept
		{
			::operator delete(p); // 18.6.1.1: doesn't throw
		}
	};

	/// A replacement for \code std::defaulte_delete \endcode that frees memory allocated with \code ::operator new[] \endcode
	template <typename T>
	class operator_delete<T[]>
	{
	public:
		constexpr operator_delete() noexcept = default;

		template <typename U>
		operator_delete(const operator_delete<U[]>& other) noexcept {}

		template <typename U>
		void operator () (U* const p) const noexcept
		{
			::operator delete[](p); // 18.6.1.2: doesn't throw
		}
	};
}
