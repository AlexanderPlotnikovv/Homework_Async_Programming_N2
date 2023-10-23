#include <iostream>
#include <future>
#include <algorithm>
#include <iostream>

std::mutex mx;

template <typename Iterator, typename Function>
void for_each_own_version(Iterator begin, Iterator end, Function func, int needed_size)
{
	auto size = std::distance(begin, end);
	if (size <= needed_size)
	{
		std::for_each(begin, end, func);
	}
	else
	{
		Iterator middle_it = std::next(begin, size / 2);
		std::future<void> fut = std::async(std::launch::async, for_each_own_version<Iterator, Function>, begin, middle_it, func, needed_size);
		fut.get();
		for_each_own_version(middle_it, end, func, needed_size);
	}
}

int main()
{
	std::vector<int> vec = { 4,1,5,2,8 };
	auto f = [](int n) { 
		std::lock_guard<std::mutex> lock(mx);
		std::cout << n << " "; 
	};
	for_each_own_version(vec.begin(), vec.end(), f, 2);
	return 0;
}