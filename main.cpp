#include <cstddef>		// std::size_t
#include <iostream>		// std::clog, std::endl

#include <vector>
#include <set>

#include <algorithm>	// std::equal, std::generate_n
#include <exception>	// std::exception
#include <random>		// std::random_device, std::default_random_engine, std::uniform_int_distribution
#include <iterator>		// std::back_inserter

#include "ReferenceAllocator.hpp"

/// Crude early tests
int main()
{
	using value_type =		int;
	using allocator_type =	cdp::ReferenceAllocator<value_type>;

	const std::vector<value_type> numbers = { 18, 13, 57, 24, 93, 80, 5, 3, 36, 71, 65, 74, 90, 38, 7, 23, 55, 35, 13, 14, 71, 88, 22, 77, 68, 100, 40, 9, 79, 30, 74, 69, 25, 98, 75, 20, 29, 14, 86, 14, 42, 16, 22, 90, 80, 36, 3, 73, 72, 38, 60, 32, 93, 64, 84, 5, 41, 64, 10, 26, 82, 5, 91, 16, 51, 21, 17, 98, 23, 56, 70, 91, 30, 23, 94, 88, 29, 24, 11, 64, 92, 68, 12, 54, 92, 65, 33, 75, 57, 59, 90, 68, 94, 46, 80, 82, 30, 86, 51, 82 };
	const std::vector<value_type> sorted_numbers = { 3, 3, 5, 5, 5, 7, 9, 10, 11, 12, 13, 13, 14, 14, 14, 16, 16, 17, 18, 20, 21, 22, 22, 23, 23, 23, 24, 24, 25, 26, 29, 29, 30, 30, 30, 32, 33, 35, 36, 36, 38, 38, 40, 41, 42, 46, 51, 51, 54, 55, 56, 57, 57, 59, 60, 64, 64, 64, 65, 65, 68, 68, 68, 69, 70, 71, 71, 72, 73, 74, 74, 75, 75, 77, 79, 80, 80, 80, 82, 82, 82, 84, 86, 86, 88, 88, 90, 90, 90, 91, 91, 92, 92, 93, 93, 94, 94, 98, 98, 100 };
	const std::vector<value_type> unique_sorted_numbers = { 3, 5, 7, 9, 10, 11, 12, 13, 14, 16, 17, 18, 20, 21, 22, 23, 24, 25, 26, 29, 30, 32, 33, 35, 36, 38, 40, 41, 42, 46, 51, 54, 55, 56, 57, 59, 60, 64, 65, 68, 69, 70, 71, 72, 73, 74, 75, 77, 79, 80, 82, 84, 86, 88, 90, 91, 92, 93, 94, 98, 100 };

	std::vector<value_type> unique_sorted_numbers2(numbers.cbegin(), numbers.cend());
	std::sort(unique_sorted_numbers2.begin(), unique_sorted_numbers2.end());
	auto last = std::unique(unique_sorted_numbers2.begin(), unique_sorted_numbers2.end());
	unique_sorted_numbers2.erase(last, unique_sorted_numbers2.end());

	try
	{
		const std::set<value_type, std::less<value_type>, allocator_type>
			tree(numbers.cbegin(), numbers.cend(), std::less<value_type>{}, allocator_type{ numbers.size() });
		/*const std::set<value_type, std::less<value_type>, allocator_type>
			tree(numbers.cbegin(), numbers.cend(), allocator_type{ numbers.size() });*/

		if (std::equal(tree.cbegin(), tree.cend(), unique_sorted_numbers.cbegin(), unique_sorted_numbers.cend()))
			std::clog << "Sorting worked correctly!" << std::endl;
		else
			std::clog << "Sorting failed to work correctly." << std::endl;
	}
	catch (const std::exception& e)
	{
		std::clog << "The operation failed, an exception was thrown : " << e.what() << std::endl;
	}
	
	return 0;
}

/*template <typename T>
void randomized_test()
{
	using value_type = int;

	constexpr std::size_t	number_of_items{ 100'000 };
	constexpr value_type	min{ 1 }, max{ 1'000'000'000 };

	std::vector<value_type> values{};
	values.reserve(number_of_items);

	std::random_device							rd{}; // seed generator
	std::default_random_engine					engine{ rd() };
	std::uniform_int_distribution<value_type>	distribution{ min, max };

	// emplace_back ?
	std::generate_n(std::back_inserter(values), number_of_items, []() { distribution(engine); });

	std::vector<value_type> sorted_values(values);
	std::sort(sorted_values.begin(), sorted_values.end());

	// Make 'tree'
}*/