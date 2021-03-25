#include<iterator>
#include<vector>
#include<algorithm>
#include <numeric>
#include<thread>
#include<iostream>
template<class InputIt, class UnaryFunction>
void for_each_parallel(InputIt first, InputIt last, UnaryFunction f)
{
    auto length = last - first;
    std::vector < std::thread > threads;
    for (std::size_t i = 0; i < std::thread::hardware_concurrency() - 1; i++)
    {
        threads.push_back(std::thread(std::for_each<InputIt, UnaryFunction>,
                                      first + i * length / (std::thread::hardware_concurrency()),
                                      first + (i + 1) * length / (std::thread::hardware_concurrency()), f));
    }
    std::for_each(first + (std::thread::hardware_concurrency() - 1) * length / (std::thread::hardware_concurrency()), last, f);
    for (std::size_t i = 0; i < std::thread::hardware_concurrency()-1; i++) {
        threads[i].join();
    }
}
int main() {
    using namespace std;
    vector<int> v(10);
    iota(v.begin(), v.end(), 1);
    for (auto i : v)
        cout << i << " ";
    cout << endl;
    for_each_parallel(v.begin(), v.end(), [](int& i) { i = i * i; });
    for (auto i : v) {
        cout << i << " ";
    }
    return 0;
}
