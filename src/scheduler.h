// Copyright (c) 2015 The Bitcoin Core developers
// Copyright (c) 2013-2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SCHEDULER_H
#define BITCOIN_SCHEDULER_H

//
// NOTE:
// Modernized to use std::thread / std::function / std::chrono for C++20.
//
#include <functional>
#include <chrono>
#include <thread>
#include <condition_variable>  // C++23 condition_variable
#include <mutex>               // C++23 mutex
#include <map>

//
// Simple class for background tasks that should be run
// periodically or once "after a while"
//
// Usage:
//
// CScheduler* s = new CScheduler();
// s->scheduleFromNow(doSomething, 11); // Assuming a: void doSomething() { }
// s->scheduleFromNow([this, arg](){ Class::func(this, arg); }, 3);
// std::thread* t = new std::thread([s]() { s->serviceQueue(); });
//
// ... then at program shutdown, clean up the thread running serviceQueue:
// t->join();
// delete t;
// delete s; // Must be done after thread is interrupted/joined.
//

class CScheduler
{
public:
    CScheduler();
    ~CScheduler();

    using Function = std::function<void(void)>;

    // Call func at/after time t
    void schedule(Function f, std::chrono::system_clock::time_point t);

    // Convenience method: call f once deltaSeconds from now
    void scheduleFromNow(Function f, int64_t deltaSeconds);

    // Another convenience method: call f approximately
    // every deltaSeconds forever, starting deltaSeconds from now.
    // To be more precise: every time f is finished, it
    // is rescheduled to run deltaSeconds later. If you
    // need more accurate scheduling, don't use this method.
    void scheduleEvery(Function f, int64_t deltaSeconds);

    // To keep things as simple as possible, there is no unschedule.

    // Services the queue 'forever'. Should be run in a thread,
    // and interrupted using stop()
    void serviceQueue();

    // Tell any threads running serviceQueue to stop as soon as they're
    // done servicing whatever task they're currently servicing (drain=false)
    // or when there is no work left to be done (drain=true)
    void stop(bool drain=false);

    // Returns number of tasks waiting to be serviced,
    // and first and last task times
    size_t getQueueInfo(std::chrono::system_clock::time_point &first,
                        std::chrono::system_clock::time_point &last) const;

private:
    std::multimap<std::chrono::system_clock::time_point, Function> taskQueue;
    std::condition_variable newTaskScheduled;  // C++23 condition_variable
    mutable std::mutex newTaskMutex;           // C++23 mutex
    int nThreadsServicingQueue;
    bool stopRequested;
    bool stopWhenEmpty;
    bool shouldStop() { return stopRequested || (stopWhenEmpty && taskQueue.empty()); }
};

#endif
