#pragma once


#include "../lib/hw.h"
#include "thread/TCB.hpp"


auto mem_alloc(size_t size) -> void*;
auto mem_free(void* ptr) -> int;

using thread_t = _thread*;

auto thread_create(thread_t* handle, void (*start_routine)(void*), void* arg) -> int;
auto thread_exit() -> int;
auto thread_dispatch() -> void;
