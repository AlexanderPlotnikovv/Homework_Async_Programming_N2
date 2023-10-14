#include <iostream>
#include <future>
#include <algorithm>
#include <iostream>

std::mutex mx;

template <typename Iterator, typename Function>
void for_each_own_version(Iterator begin, Iterator end, Function func)
{
	auto size = std::distance(begin, end);
	std::vector<std::future<void>> future(size);
	int i = 0;
	for (auto it = begin; it != end; ++it)
	{
		future[i] = std::async(std::launch::async,func, *it);
		++i;
	}
	for (auto& fut : future)
	{
		fut.get();
	}
}

int main()
{
	std::vector<int> vec = { 4,1,5,2,8 };
	auto f = [](int n) { 
		std::lock_guard<std::mutex> lock(mx);
		std::cout << n << " "; 
	};
	for_each_own_version(vec.begin(), vec.end(), f);
	return 0;
}