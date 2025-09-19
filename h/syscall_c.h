#pragma once

#include "semaphore.hpp"
#include "../lib/hw.h"
#include "thread.hpp"

constexpr auto EOF = -1;

using thread_t = _thread*;
using sem_t = _sem*;
using time_t = unsigned long;

auto mem_alloc(size_t size) -> void*;
auto mem_free(void* pointer) -> int;
auto mem_get_free_space() -> size_t;
auto mem_get_largest_free_block() -> size_t;

auto thread_create(thread_t* handle, void (*start_routine)(void*), void* arg) -> int;
auto thread_exit() -> int;
auto thread_dispatch() -> void;

auto sem_open(sem_t* handle, unsigned init) -> int;
auto sem_close(sem_t handle) -> int;
auto sem_wait(sem_t id) -> int;
auto sem_signal(sem_t id) -> int;

auto time_sleep(time_t timeout) -> int;

auto getc() -> char;
auto putc(char chr) -> void;