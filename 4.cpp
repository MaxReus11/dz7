#include <thread>
#include <utility>
#include <random>
#include <vector>
#include <algorithm>
#include "Timer.h"
#include <future>
#include <iterator>
#include <numeric>
#include <execution>
#include <iostream>
#include <math.h>
#include <numeric>

int main()
{
	std::vector<double> testa, testb;
	testa.reserve(100000000);
	testb.reserve(100000000);
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_real_distribution<double> dis(0, 1);
	for (int i = 0; i < 100000000; i++) {
		testa.push_back(dis(g));
	}
	std::copy(testa.begin(), testa.end(),std::back_inserter(testb));
	{
		Timer a;
		std::for_each(testa.begin(), testa.end(), [](double& i) {i = std::sin(i); });
		std::cout << "Time on for_each " << a.print() << std::endl;//slower 5000000 microsec 
	}
	{
		Timer a;
		std::for_each(std::execution::par, testb.begin(), testb.end(), [](double& i) {i = std::sin(i); });
		std::cout << "Time on parallel for_each "<<a.print() << std::endl;// faster 650000 microsec
	}
	{
		Timer a;
		std::partial_sum(testa.begin(), testa.end(), testa.begin(), std::multiplies<>());
		std::cout << "Time on partial sum " << a.print() << std::endl;//125000ms
	}
	{
		Timer a;
		std::inclusive_scan(std::execution::par, testb.begin(), testb.end(), testb.begin(), std::multiplies<>());
		std::cout << "Time on parallel scan " << a.print() << std::endl; // 145000ms
	}
	
	{
		Timer a;
		auto result = std::transform_reduce(std::execution::par, testa.begin(), testa.end(), testb.begin(),0.0);
		std::cout <<result <<" Time on parallel transform " << a.print() << std::endl;//55000ms
	}
	
	{
		Timer a;
		auto result=std::inner_product(testa.begin(), testa.end(),testb.begin(), 0.0);
		std::cout <<result<<" Time on  inner " << a.print() << std::endl; //300000 ms
	}


}
