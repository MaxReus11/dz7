#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include<fstream>
#include "timer.h"
class Threads_Guard
{
public:

    explicit Threads_Guard(std::vector < std::thread > & threads) :
            m_threads(threads)
    {}

    Threads_Guard(Threads_Guard const&) = delete;

    Threads_Guard& operator=(Threads_Guard const&) = delete;

    ~Threads_Guard() noexcept
    {
        try
        {
            for (std::size_t i = 0; i < m_threads.size(); ++i)
            {
                if (m_threads[i].joinable())
                {
                    m_threads[i].join();
                }
            }
        }
        catch (...)
        {
            // std::abort();
        }
    }

private:

    std::vector < std::thread > & m_threads;
};

template < typename Iterator, typename T >
struct accumulate_block
{
    T operator()(Iterator first, Iterator last)
    {
        return std::accumulate(first, last, T());
    }
};

template < typename Iterator, typename T >
T parallel_accumulate(Iterator first, Iterator last, T init, int N)
{
    const std::size_t length = std::distance(first, last);

    if (!length)
        return init;

    //const std::size_t min_per_thread = 25;
    //const std::size_t max_threads = (length + min_per_thread - 1) / min_per_thread;

    //const std::size_t hardware_threads =std::thread::hardware_concurrency();

    const std::size_t num_threads =
            (N != 0 ? N : 2);

    const std::size_t block_size = length / num_threads;

    std::vector < std::future < T > > futures(num_threads - 1);
    std::vector < std::thread >		  threads(num_threads - 1);

    Threads_Guard guard(threads);

    Iterator block_start = first;

    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);

        std::packaged_task < T(Iterator, Iterator) > task{
                accumulate_block < Iterator, T >() };

        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task), block_start, block_end);

        block_start = block_end;
    }

    T last_result = accumulate_block < Iterator, T > ()(block_start, last);

    T result = init;

    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {
        result += futures[i].get();
    }

    result += last_result;

    return result;
}

int main(int argc, char ** argv)
{
    using namespace std;
    vector < int > v(100);
    int N;
    cout<<"Enter the number of threads: ";
    cin>>N;
    iota(v.begin(), v.end(), 1);
    Timer parallel;
    cout <<"A sum of the vector: "<<parallel_accumulate(v.begin(), v.end(), 0, N) << std::endl;
    time_t par = parallel.print();
    cout<<"Time of parallel accumulate: "<<par<<endl;
    fstream fout("data.txt", ios::app);
    if(!fout.is_open()){
        cerr<<"Can't open the file \"data.txt\"!"<<endl;
        return (EXIT_FAILURE);
    }
    fout<<N<<";"<<par<<"\n";
    fout.close();
    return EXIT_SUCCESS;
}
