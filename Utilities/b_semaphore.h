//
// Created by Daniel on 17.05.2019.
//

#ifndef BOOKSTORE_B_SEMAPHORE_H
#define BOOKSTORE_B_SEMAPHORE_H

#include <mutex>

class b_semaphore {
public:
    using Mutex = std::mutex;
    using lock = std::unique_lock<Mutex>;

    explicit b_semaphore(int count = 0);

    b_semaphore(const b_semaphore &) = delete;

    b_semaphore(b_semaphore &&) = delete;

    b_semaphore &operator=(const b_semaphore &) = delete;

    b_semaphore &operator=(b_semaphore &&) = delete;

    void up();

    void down();

    bool try_down();

private:
    Mutex m_mutex;
    int m_count;

};


#endif //BOOKSTORE_B_SEMAPHORE_H
