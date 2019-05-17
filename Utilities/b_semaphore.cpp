//
// Created by Daniel on 17.05.2019.
//

#include "b_semaphore.h"

b_semaphore::b_semaphore(int count) : m_count{count} {}

void b_semaphore::up() {
    std::unique_lock<Mutex> lock{m_mutex};
    ++m_count;
}

void b_semaphore::down() {
    std::unique_lock<Mutex> lock{m_mutex};
    --m_count;
}

bool b_semaphore::try_down() {

    std::unique_lock<Mutex> lock{m_mutex};

    if (m_count > 0) {
        --m_count;
        return true;
    }
    return false;
}
