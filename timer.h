#ifndef UNTITLED_TIMER_H
#define UNTITLED_TIMER_H
#include <chrono>
#include <iostream>
#include <algorithm>


class Timer {
public:
    using clock_t = std::chrono::steady_clock;
    using time_point_t = clock_t::time_point;
    Timer() : m_begin(clock_t::now()) {
    };
    auto print() {
        auto end = clock_t::now();
        return std::chrono::duration_cast<std::chrono::microseconds> (end - m_begin).count();

    }
    ~Timer() {
    }
private:
    time_point_t m_begin;
};

#endif
