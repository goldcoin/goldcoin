// Copyright (c) 2007-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2011-2017 The Litecoin Core developers
// Copyright (c) 2013-2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#if defined(HAVE_CONFIG_H)
#include "config/bitcoin-config.h"
#endif

#include "utiltime.h"

#include <chrono>  // C++23 chrono
#include <thread>  // C++23 thread
#include <ctime>
#include <cassert>

using namespace std;

static int64_t nMockTime = 0; //!< For unit testing

int64_t GetTime()
{
    if (nMockTime) return nMockTime;

    time_t now = time(nullptr);
    assert(now > 0);
    return now;
}

void SetMockTime(int64_t nMockTimeIn)
{
    nMockTime = nMockTimeIn;
}

int64_t GetTimeMillis()
{
    // C++23: Replace boost::posix_time with std::chrono
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    int64_t millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    assert(millis > 0);
    return millis;
}

int64_t GetTimeMicros()
{
    // C++23: Replace boost::posix_time with std::chrono
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    int64_t micros = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    assert(micros > 0);
    return micros;
}

int64_t GetSystemTimeInSeconds()
{
    return GetTimeMicros()/1000000;
}

/** Return a time useful for the debug log */
int64_t GetLogTimeMicros()
{
    if (nMockTime) return nMockTime*1000000;

    return GetTimeMicros();
}

void MilliSleep(int64_t n)
{
    // C++23: Always use std::this_thread::sleep_for
    std::this_thread::sleep_for(std::chrono::milliseconds(n));
}

std::string DateTimeStrFormat(const char* pszFormat, int64_t nTime)
{
    time_t time = (time_t)nTime;
    struct tm* tm = gmtime(&time);
    if (!tm) return "";
    
    char buffer[200];
    size_t ret = strftime(buffer, sizeof(buffer), pszFormat, tm);
    return ret ? std::string(buffer) : "";
}
