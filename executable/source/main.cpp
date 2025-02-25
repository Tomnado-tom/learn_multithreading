#include <iostream>
#include <library/library.h>
#include <atomic>
#include <thread>
#include <latch>

std::atomic_bool shared_flag;
int shared_data = 0;

std::latch work_done1{ 5 };
std::latch work_done2{ 5 };

void IncrementSharedRelaxedIssues()
{
    work_done1.arrive_and_wait();
    for (int count{ 0 }; count < 1000000;)
    {
        bool expected = false;
        if (shared_flag.compare_exchange_strong(expected, true, std::memory_order_relaxed))
        {
            shared_data++;
            count++;
            shared_flag.store(false, std::memory_order_relaxed);
        }
    }
}

void IncrementSharedFence()
{
    work_done2.arrive_and_wait();
    for (int count{ 0 }; count < 1000000;)
    {
        bool expected = false;
        if (shared_flag.compare_exchange_strong(expected, true, std::memory_order_acquire))
        {
            shared_data++;
            count++;
            shared_flag.store(false, std::memory_order_release);
        }
    }
}


int main()
{
    {

        std::jthread thread1(&IncrementSharedRelaxedIssues);
        std::jthread thread2(&IncrementSharedRelaxedIssues);
        std::jthread thread3(&IncrementSharedRelaxedIssues);
        std::jthread thread4(&IncrementSharedRelaxedIssues);
        std::jthread thread5(&IncrementSharedRelaxedIssues);
    }

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

    return 0;
}