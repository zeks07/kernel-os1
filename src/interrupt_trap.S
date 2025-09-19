.global trap_vector_base
.type trap_vector_base @function

.align 4

trap_vector_base:
    addi sp, sp, -256
    .irp index 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    sd x\index, \index * 8(sp)
    .endr

    # save sstatus
    csrr s1, sstatus

    # read scause
    csrr t2, scause

    # ecall in user mode
    li t1, 8
    beq t2, t1, syscall_dispatch

    # ecall in system mode
    li t1, 9
    beq t2, t1, syscall_dispatch

    # timer
    li t1, 0x8000000000000001
    beq t2, t1, handle_timer

    # console
    li t1, 0x8000000000000009
    beq t2, t1, console

    j illegal_instructions

syscall_dispatch:
    # sepc += 4
    csrr t0, sepc
    addi t0, t0, 4
    csrw sepc, t0

    # operator code switch clause
    li t0, 0x01
    beq a0, t0, mem_alloc

    li t0, 0x02
    beq a0, t0, mem_free

    li t0, 0x03
    beq a0, t0, mem_get_free_space

    li t0, 0x04
    beq a0, t0, mem_get_largest_free_block

    li t0, 0x11
    beq a0, t0, thread_create

    li t0, 0x12
    beq a0, t0, thread_exit

    li t0, 0x13
    beq a0, t0, thread_dispatch

    li t0, 0x21
    beq a0, t0, sem_open

    li t0, 0x22
    beq a0, t0, sem_close

    li t0, 0x23
    beq a0, t0, sem_wait

    li t0, 0x24
    beq a0, t0, sem_signal

    li t0, 0x31
    beq a0, t0, time_sleep

    li t0, 0x41
    beq a0, t0, getc

    li t0, 0x42
    beq a0, t0, putc

    j invalid_operator_code

# kernel::memory::allocate(unsigned long)
.extern _ZN6kernel6memory8allocateEm
mem_alloc:
    mv a0, a1   # size_t size
    call _ZN6kernel6memory8allocateEm
    j syscall_done

# kernel::memory::free(void*)
.extern _ZN6kernel6memory4freeEPv
mem_free:
    mv a0, a1   # void* pointer
    call _ZN6kernel6memory4freeEPv
    j syscall_done

# kernel::memory::get_free_space
.extern _ZN6kernel6memory14get_free_spaceEv
mem_get_free_space:
    call _ZN6kernel6memory14get_free_spaceEv
    j syscall_done

# kernel::memory::get_largest_free_block
.extern _ZN6kernel6memory22get_largest_free_blockEv
mem_get_largest_free_block:
    call _ZN6kernel6memory22get_largest_free_blockEv
    j syscall_done

# kernel::thread::create(kernel::thread::TCB**, void (*)(void*), void*)
.extern _ZN6kernel6thread6createEPPNS0_3TCBEPFvPvES4_
thread_create:
    mv a0, a1   # kernel::thread::TCB** handle
    mv a1, a2   # void (* body)(void *)
    mv a2, a3   # void* arg
    call _ZN6kernel6thread6createEPPNS0_3TCBEPFvPvES4_
    j syscall_done

# kernel::thread::exit()
.extern _ZN6kernel6thread4exitEv
thread_exit:
    call _ZN6kernel6thread4exitEv
    j syscall_done

# kernel::thread::dispatch()
.extern _ZN6kernel6thread8dispatchEv
thread_dispatch:
    call _ZN6kernel6thread8dispatchEv
    j syscall_done

# kernel::sync::open_semaphore(kernel::sync::Semaphore**, unsigned int)
.extern _ZN6kernel4sync14open_semaphoreEPPNS0_9SemaphoreEj
sem_open:
    mv a0, a1   # kernel::sync::Semaphore** handle
    mv a1, a2   # unsigned int init
    call _ZN6kernel4sync14open_semaphoreEPPNS0_9SemaphoreEj
    j syscall_done

# kernel::sync::Semaphore::close()
.extern _ZN6kernel4sync9Semaphore5closeEv
sem_close:
    mv a0, a1   # this
    call _ZN6kernel4sync9Semaphore5closeEv
    j syscall_done

# kernel::sync::Semaphore::wait()
.extern _ZN6kernel4sync9Semaphore4waitEv
sem_wait:
    mv a0, a1   #this
    call _ZN6kernel4sync9Semaphore4waitEv
    j syscall_done

# kernel::sync::Semaphore::signal()
.extern _ZN6kernel4sync9Semaphore6signalEv
sem_signal:
    mv a0, a1   #this
    call _ZN6kernel4sync9Semaphore6signalEv
    j syscall_done

# kernel::thread::sleep(unsigned long)
# .extern _ZN6kernel6thread5sleepEm
time_sleep:
    mv a0, a1   # unsigned long timeout
    # call _ZN6kernel6thread5sleepEm
    li a0, 1
    j syscall_done

# kernel::console::get_char()
.extern _ZN6kernel7console8get_charEv
getc:
    call _ZN6kernel7console8get_charEv
    j syscall_done

# kernel::console::put_char(char)
.extern _ZN6kernel7console8put_charEc
putc:
    mv a0, a1   # chr char
    call _ZN6kernel7console8put_charEc
    j syscall_done

invalid_operator_code:
    li a0, -1

syscall_done:
    sd a0, 10 * 8(sp)
    j exit_trap

# kernel::thread::tick
# .extern _ZN6kernel6thread4tickEv
handle_timer:
    # call _ZN6kernel6thread4tickEv
    li t0, 0x2
    csrc sip, t0
    j exit_trap

# kernel::console::handle_interrupt()
.extern _ZN6kernel7console16handle_interruptEv
console:
    call _ZN6kernel7console16handle_interruptEv
    j exit_trap

# kernel::error(unsinged long, unsinged long)
.extern _ZN6kernel5errorEmm
illegal_instructions:
    mv a0, s1   # unsigned long status
    mv a1, t2   # unsigned long cause
    call _ZN6kernel5errorEmm
    j exit_trap

exit_trap:
    csrw sstatus, s1

    .irp index 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256

    sret