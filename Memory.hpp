#pragma once

#include <new>		// operator delete, operator delete[]

namespace cdp
{
	// Default deleters: 20.8.1.1

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
