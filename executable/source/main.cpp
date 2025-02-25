#include <iostream>
#include <library/library.h>
#include <atomic>
#include <thread>

std::atomic_bool shared_flag;
int shared_data = 0;


void IncrementSharedRelaxedIssues()
{
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
    }

    std::cout << "Shared data: " << shared_data << std::endl;
    shared_data = 0;
    {
        std::jthread thread1(&IncrementSharedRelaxedIssues);
        std::jthread thread2(&IncrementSharedRelaxedIssues);
    }

    std::cout << "Shared data: " << shared_data << std::endl;

    return 0;
}