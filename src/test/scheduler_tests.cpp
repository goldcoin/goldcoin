// Copyright (c) 2012-2016 The Bitcoin Core developers
// Copyright (c) 2013-2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "random.h"
#include "scheduler.h"

#include "test/test_bitcoin.h"

#include <boost/bind/bind.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/thread.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(scheduler_tests)

static void microTask(CScheduler& s, std::mutex& mutex, int& counter, int delta, std::chrono::system_clock::time_point rescheduleTime)  // C++23 mutex
{
    {
        std::unique_lock<std::mutex> lock(mutex);  // C++23 lock
        counter += delta;
    }
    std::chrono::system_clock::time_point noTime = std::chrono::system_clock::time_point::min();
    if (rescheduleTime != noTime) {
        CScheduler::Function f = [&s, &mutex, &counter, delta](){ microTask(s, mutex, counter, -delta + 1, {}); };  // C++23 lambda
        s.schedule(f, rescheduleTime);
    }
}

static void MicroSleep(uint64_t n)
{
#if defined(HAVE_WORKING_BOOST_SLEEP_FOR)
    std::this_thread::sleep_for(std::chrono::microseconds(n));
#elif defined(HAVE_WORKING_BOOST_SLEEP)
    std::this_thread::sleep_for(std::chrono::microseconds(n));  // C++23 sleep
#else
    //should never get here
    #error missing boost sleep implementation
#endif
}

BOOST_AUTO_TEST_CASE(manythreads)
{
    // Stress test: hundreds of microsecond-scheduled tasks,
    // serviced by 10 threads.
    //
    // So... ten shared counters, which if all the tasks execute
    // properly will sum to the number of tasks done.
    // Each task adds or subtracts a random amount from one of the
    // counters, and then schedules another task 0-1000
    // microseconds in the future to subtract or add from
    // the counter -random_amount+1, so in the end the shared
    // counters should sum to the number of initial tasks performed.
    CScheduler microTasks;

    std::mutex counterMutex[10];  // C++23 mutex array
    int counter[10] = { 0 };
    std::mt19937 rng(42);  // C++23 random
    std::uniform_int_distribution<> zeroToNine(0, 9);
    std::uniform_int_distribution<> randomMsec(-11, 1000);
    std::uniform_int_distribution<> randomDelta(-1000, 1000);

    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point now = start;
    std::chrono::system_clock::time_point first, last;
    size_t nTasks = microTasks.getQueueInfo(first, last);
    BOOST_CHECK(nTasks == 0);

    for (int i = 0; i < 100; i++) {
        std::chrono::system_clock::time_point t = now + std::chrono::microseconds(randomMsec(rng));
        std::chrono::system_clock::time_point tReschedule = now + std::chrono::microseconds(500 + randomMsec(rng));
        int whichCounter = zeroToNine(rng);
        CScheduler::Function f = [&microTasks, &counterMutex, &counter, whichCounter, randomDelta](){ 
            microTask(microTasks, counterMutex[whichCounter], counter[whichCounter], randomDelta,
                                             randomDelta(rng), tReschedule);
        microTasks.schedule(f, t);
    }
    nTasks = microTasks.getQueueInfo(first, last);
    BOOST_CHECK(nTasks == 100);
    BOOST_CHECK(first < last);
    BOOST_CHECK(last > now);

    // As soon as these are created they will start running and servicing the queue
    std::vector<std::thread> microThreads;  // C++23 thread group
    for (int i = 0; i < 5; i++)
        microThreads.emplace_back([&microTasks](){ microTasks.serviceQueue(); });  // C++23 thread

    MicroSleep(600);
    now = std::chrono::system_clock::now();

    // More threads and more tasks:
    for (int i = 0; i < 5; i++)
        microThreads.emplace_back([&microTasks](){ microTasks.serviceQueue(); });  // C++23 thread
    for (int i = 0; i < 100; i++) {
        std::chrono::system_clock::time_point t = now + std::chrono::microseconds(randomMsec(rng));
        std::chrono::system_clock::time_point tReschedule = now + std::chrono::microseconds(500 + randomMsec(rng));
        int whichCounter = zeroToNine(rng);
        CScheduler::Function f = [&microTasks, &counterMutex, &counter, whichCounter, randomDelta](){ 
            microTask(microTasks, counterMutex[whichCounter], counter[whichCounter], randomDelta,
                                             randomDelta(rng), tReschedule);
        microTasks.schedule(f, t);
    }

    // Drain the task queue then exit threads
    microTasks.stop(true);
    microThreads.join_all(); // ... wait until all the threads are done

    int counterSum = 0;
    for (int i = 0; i < 10; i++) {
        BOOST_CHECK(counter[i] != 0);
        counterSum += counter[i];
    }
    BOOST_CHECK_EQUAL(counterSum, 200);
}

BOOST_AUTO_TEST_SUITE_END()
