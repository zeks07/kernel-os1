#include "../h/syscall_c.h"

#include "printing.hpp"

static volatile bool finished[2];

static void sleepyRun(void *arg) {
    time_t sleep_time = *((time_t *) arg);
    int i = 6;

    auto id = sleep_time/10-1;

    println("%d is off to bed", id);

    while (--i > 0) {
        printString("Hello ");
        printInt(sleep_time);
        printString(" !\n");
        time_sleep(sleep_time);
        println("%d is awake", id);
    }
    finished[id] = true;
}

void testSleeping() {
    println("Hello!");

    const int sleepy_thread_count = 2;
    time_t sleep_times[sleepy_thread_count] = {10, 20};
    thread_t sleepyThread[sleepy_thread_count];

    println("Let's start");

    for (int i = 0; i < sleepy_thread_count; i++) {
        thread_create(&sleepyThread[i], sleepyRun, sleep_times + i);
    }

    while (!(finished[0] && finished[1])) {
        thread_dispatch();
    }
}
