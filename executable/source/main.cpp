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

void IncrementSharedUnsafe()
{
    barrier.arrive_and_wait();
    for (int count{ 0 }; count < 1000000;)
    {
        shared_data++;
        --shared_data;
        count++;
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
    std::jthread thread1(function);
    std::jthread thread2(function);
    std::jthread thread3(function);
    std::jthread thread4(function);
    std::jthread thread5(function);

    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    std::cout << "Elapsed time: " << elapsed_time.count() << " seconds" << std::endl;
}


int main()
{
    RunThread(&IncrementSharedRelaxedIssues);

    std::cout << "Shared data: " << shared_data << std::endl;


    shared_data = 0;
    {
        std::jthread thread1(&IncrementSharedFence);
        std::jthread thread2(&IncrementSharedFence);
        std::jthread thread3(&IncrementSharedFence);
        std::jthread thread4(&IncrementSharedFence);
        std::jthread thread5(&IncrementSharedFence);
    }

    std::cout << "Shared data: " << shared_data << std::endl;

    shared_data = 0;
    {
        std::jthread thread1(&IncrementSharedWithMutex);
        std::jthread thread2(&IncrementSharedWithMutex);
        std::jthread thread3(&IncrementSharedWithMutex);
        std::jthread thread4(&IncrementSharedWithMutex);
        std::jthread thread5(&IncrementSharedWithMutex);
    }
    std::cout << "Shared data: " << shared_data << std::endl;
    shared_data = 0;
    {
        std::jthread thread1(&IncrementSharedUnsafe);
        std::jthread thread2(&IncrementSharedUnsafe);
        std::jthread thread3(&IncrementSharedUnsafe);
        std::jthread thread4(&IncrementSharedUnsafe);
        std::jthread thread5(&IncrementSharedUnsafe);
    }
    std::cout << "Shared data: " << shared_data << std::endl;

    return 0;
}