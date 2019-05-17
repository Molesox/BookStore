//
// Created by Daniel on 16.05.2019.
//

#ifndef BOOKSTORE_BASIC_SEMAPHORE_H
#define BOOKSTORE_BASIC_SEMAPHORE_H


#include <mutex>
#include <condition_variable>


class basic_semaphore {
public:
    using Mutex = std::mutex;
    using lock = std::unique_lock<Mutex>;
    using CondVar = std::condition_variable;

    explicit basic_semaphore(int count = 0);

    basic_semaphore(const basic_semaphore &) = delete;

    basic_semaphore(basic_semaphore &&) = delete;

    basic_semaphore &operator=(const basic_semaphore &) = delete;

    basic_semaphore &operator=(basic_semaphore &&) = delete;

    void notify();

    void wait();

    void up();

    void down();

    bool try_wait();

    bool wait_for(const std::chrono::duration<float, std::milli> &d);

    template<class Clock, class Duration>
    bool wait_until(const std::chrono::time_point<Clock, Duration> &t);


private:
    Mutex mMutex;
    CondVar mCv;
    int mCount;


};


#endif //BOOKSTORE_BASIC_SEMAPHORE_H
