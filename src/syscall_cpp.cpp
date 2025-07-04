#include "../h/syscall_cpp.hpp"

auto operator new(const size_t size) -> void* { return mem_alloc(size); }

auto operator delete(void* ptr) -> void { mem_free(ptr); }

Thread::Thread(void (*body)(void*), void* arg)
    : my_handle(nullptr), my_body(body), my_arg(arg) {}

Thread::~Thread() { delete my_handle; }

int Thread::start() { return thread_create(&my_handle, my_body, my_arg); }

void Thread::dispatch() { thread_dispatch(); }

Thread::Thread()
    : my_handle(nullptr),
      my_body([](void* arg) -> void { static_cast<Thread*>(arg)->run(); }),
      my_arg(this) {}
