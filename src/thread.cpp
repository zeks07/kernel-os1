#include "../h/thread.hpp"

#include "../h/syscall_c.h"
#include "../h/collections/List.hpp"
// #include "../h/collections/SortedList.hpp"

namespace kernel {
  namespace thread {
    namespace {
      constexpr auto compare(const time_t a, const time_t b) -> bool { return a < b; }

      collection::List<TCB*>* ready_queue;
      // collection::SortedList<TCB*, time_t, compare>* sleep_queue;
      TCB* running_thread;
      // unsigned long long time;
      // time_t time_since_last_dispatch;

      auto pop_spp_spie() -> void {
        __asm__ volatile ("csrw sepc, ra");
        __asm__ volatile ("sret");
      }

      auto thread_wrapper() -> void {
        pop_spp_spie();

        running_thread->run();

        thread_exit();
      }
    }

    TCB::TCB(const int id, const body body, void* argument, const size_t stack_size)
      : id(id), state(State::Ready), thread_body(body), thread_arguments(argument) {
      if (!body) {
        stack = nullptr;
        this->stack_size = 0;
        context = {0, 0};
        return;
      }

      stack = mem_alloc(stack_size);
      this->stack_size = stack_size;
      context = {
        reinterpret_cast<uint64>(&thread_wrapper),
        reinterpret_cast<uint64>(stack) + stack_size - 1
      };
    }

    auto TCB::get_id() const -> int {
      return id;
    }

    auto TCB::get_state() const -> State {
      return state;
    }

    auto TCB::wake(const WakeReason reason) -> void {
      state = State::Ready;
      wake_reason = reason;
      put(this);
    }

    auto TCB::block() -> void {
      state = State::Waiting;
    }

    auto TCB::finish() -> void {
      state = State::Finished;
    }

    auto TCB::get_wake_reason() const -> WakeReason {
      return wake_reason;
    }

    auto TCB::run() const -> void {
      thread_body(thread_arguments);
    }

    TCB::~TCB() {
      mem_free(stack);
    }

    auto init() -> void {
      thread_t kernel_thread;
      thread_create(&kernel_thread, nullptr, nullptr);
      ready_queue = new collection::List<TCB*>;
      // sleep_queue = new collection::SortedList<TCB*, time_t, compare>;
      running_thread = kernel_thread;
      // time = 0;
      // time_since_last_dispatch = 0;
    }

    // ReSharper disable once CppDFAConstantFunctionResult
    auto create(TCB** handle, const body body, void* arg) -> int {
      static auto id = 0;

      const auto new_thread = new TCB(id++, body, arg, DEFAULT_STACK_SIZE);
      // ReSharper disable once CppDFAConstantConditions CppDFAUnreachableCode
      if (!new_thread) return -1;

      if (body) put(new_thread);
      *handle = new_thread;
      return 0;
    }

    // auto sleep(const time_t timeout) -> void {
    //   running_thread->block();
    //   sleep_queue->add(running_thread, time + timeout);
    //   thread_dispatch();
    // }

    auto exit() -> void {
      running_thread->finish();
      thread_dispatch();
    }

    auto put(TCB* thread) -> void {
      ready_queue->add(thread);
    }

    auto get() -> TCB* {
      if (ready_queue->is_empty()) return nullptr;
      return ready_queue->remove_first();
    }

    auto dispatch() -> void {
      const auto old_thread = running_thread;
      if (old_thread->get_state() == State::Ready) put(old_thread);
      running_thread = get();
      // time_since_last_dispatch = 0;
      switch_context(old_thread, running_thread);
    }

    // auto tick() -> void {
    //   time++;
    //   if (!sleep_queue->is_empty() && sleep_queue->get_key(0) <= time) {
    //     const auto thread = sleep_queue->remove_first();
    //     thread->wake(WakeReason::Timeout);
    //   }
    //   time_since_last_dispatch++;
    //   if (time_since_last_dispatch >= 100) {
    //     thread_dispatch();
    //   }
    // }

    auto get_running() -> TCB* {
      return running_thread;
    }
  }
}
