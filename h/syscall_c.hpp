#pragma once


#include "../lib/hw.h"
#include "sync/Semaphore.hpp"
#include "thread/TCB.hpp"


using thread_t = _thread*;
using sem_t = _sem*;
using time_t = unsigned long;

auto mem_alloc(size_t size) -> void*;
auto mem_free(void* ptr) -> int;

auto thread_create(thread_t* handle, void (*start_routine)(void*), void* arg) -> int;
auto thread_exit() -> int;
auto thread_dispatch() -> void;

auto sem_open(sem_t* handle, unsigned init) -> int;
auto sem_close(sem_t handle) -> int;
auto sem_wait(sem_t id) -> int;
auto sem_signal(sem_t id) -> int;
auto sem_timedwait(sem_t id, time_t timeout) -> int;
auto sem_trywait(sem_t id) -> int;

/**
 * 1 time_t = 100 milliseconds
 */
auto time_sleep(time_t timeout) -> int;
