#ifndef SPINLOCK_H
#define SPINLOCK_H
#include <sched.h>

namespace Memory{

struct Spinlock {
    Spinlock(bool yield = true)
        : value(0), yield(yield)
    {
    }

    inline void lock()
    {
        acquire();
    }

    inline void unlock()
    {
        release();
    }

    bool locked() const
    {
        return value;
    }

    int try_acquire()
    {
        if (__sync_bool_compare_and_swap(&value, 0, 1))
            return 0;
        return -1;
    }

    bool try_lock()
    {
        return try_acquire() == 0;
    }

    int acquire()
    {
        for (int tries = 0; true;  ++tries) {
            if (!__sync_lock_test_and_set(&value, 1))
                return 0;
            if (tries == 100 && yield) {
                tries = 0;
                sched_yield();
            }
        }
    }

    int release()
    {
        __sync_lock_release(&value);
        return 0;
    }

    volatile int value;
    bool yield;
};


} // namespace util

#endif
