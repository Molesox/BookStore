//
// Created by Daniel on 16.05.2019.
//

#include "basic_semaphore.h"

void basic_semaphore::up() {
    std::unique_lock<Mutex> lock{mMutex};
    ++mCount;
    lock.unlock();
}

void basic_semaphore::down() {
    std::unique_lock<Mutex> lock{mMutex};
    --mCount;
    lock.unlock();
}


basic_semaphore::basic_semaphore(int count)
        : mCount{count} {}


void basic_semaphore::wait() {
    std::unique_lock<Mutex> lock{mMutex};
    mCv.wait(lock, [&] { return mCount > 0; });
    --mCount;
}


bool basic_semaphore::try_wait() {
    std::unique_lock<Mutex> lock{mMutex};

    if (mCount > 0) {
        --mCount;
        return true;
    }

    return false;
}


bool basic_semaphore::wait_for(const std::chrono::duration<float, std::milli> &d) {
    std::unique_lock<Mutex> lock{mMutex};
    auto finished = mCv.wait_for(lock, d, [&] { return mCount > 0; });

    if (finished)
        --mCount;

    return finished;
}


template<class Clock, class Duration>
bool basic_semaphore::wait_until(const std::chrono::time_point<Clock, Duration> &t) {
    std::unique_lock<Mutex> lock{mMutex};
    auto finished = mCv.wait_until(lock, t, [&] { return mCount > 0; });

    //if (finished)
    --mCount;

    return finished;
}

void basic_semaphore::notify() {
    std::lock_guard<Mutex> lock{mMutex};
    ++mCount;
    mCv.notify_one();
}
