#include <iostream>
#include <library/library.h>
#include <atomic>
#include <thread>
#include <barrier>
#include <mutex>
#include <chrono>

std::atomic_bool shared_flag;
uint64_t shared_data = 0;

std::barrier barrier{ 5 };


void IncrementSharedRelaxedIssuesWeak()
{
    barrier.arrive_and_wait();
    for (int count{ 0 }; count < 1000000;)
    {
        bool expected = false;
        //this doesn't work. Which is expected.
        while (!shared_flag.compare_exchange_weak(expected, true, std::memory_order_relaxed, std::memory_order_relaxed) && !expected);

        shared_data++;
        --shared_data;
        count++;
        shared_flag.store(false, std::memory_order_relaxed);
    }
}

void IncrementSharedFenceWeak()
{
    barrier.arrive_and_wait();
    for (int count{ 0 }; count < 1000000;)
    {
        bool expected = false;
        //this doesn't work but i'm not sure why.
        //maybe only use compare_exchange_weak for single data swaps. e.g swapping head of a stack.
        while (!shared_flag.compare_exchange_weak(expected, true, std::memory_order_acquire) && !expected);

        shared_data++;
        --shared_data;
        count++;
        shared_flag.store(false, std::memory_order_release);
    }
}

void IncrementSharedRelaxedIssues()
{
    barrier.arrive_and_wait();
    for (int count{ 0 }; count < 1000000;)
    {
        bool expected = false;
        if (shared_flag.compare_exchange_strong(expected, true, std::memory_order_relaxed))
        {
            shared_data++;
            --shared_data;
            count++;
            shared_flag.store(false, std::memory_order_relaxed);
        }
    }
}

void IncrementSharedFence()
{
    barrier.arrive_and_wait();
    for (int count{ 0 }; count < 1000000;)
    {
        bool expected = false;
        if (shared_flag.compare_exchange_strong(expected, true, std::memory_order_acquire))
        {
            shared_data++;
            --shared_data;
            count++;
            shared_flag.store(false, std::memory_order_release);
        }
    }
}

std::mutex shared_mutex;

void IncrementSharedWithMutex()
{
    barrier.arrive_and_wait();
    for (int count{ 0 }; count < 1000000;)
    {
        std::lock_guard<std::mutex> lock(shared_mutex);
        shared_data++;
        --shared_data;
        count++;
    }
}

template<typename fn>
void RunThread(fn&& function)
{
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    {
        std::jthread thread1(function);
        std::jthread thread2(function);
        std::jthread thread3(function);
        std::jthread thread4(function);
        std::jthread thread5(function);
    }
    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    std::cout << "Elapsed time: " << elapsed_time.count() << " seconds" << std::endl;
}


int main()
{
    std::cout << "IncrementSharedWithMutex: " << std::endl;
    RunThread(&IncrementSharedWithMutex);
    std::cout << "Shared data: " << shared_data << std::endl;
    shared_data = 0;

    std::cout << "IncrementSharedRelaxedIssues: " << std::endl;
    RunThread(&IncrementSharedRelaxedIssues);
    std::cout << "Shared data: " << shared_data << std::endl;
    shared_data = 0;

    std::cout << "IncrementSharedFence: " << std::endl;
    RunThread(&IncrementSharedFence);
    std::cout << "Shared data: " << shared_data << std::endl;
    shared_data = 0;

    std::cout << "IncrementSharedRelaxedIssuesWeak: " << std::endl;
    RunThread(&IncrementSharedRelaxedIssuesWeak);
    std::cout << "Shared data: " << shared_data << std::endl;
    shared_data = 0;

    std::cout << "IncrementSharedFenceWeak: " << std::endl;
    RunThread(&IncrementSharedFenceWeak);
    std::cout << "Shared data: " << shared_data << std::endl;
    shared_data = 0;

    return 0;
}