.extern _ZN6kernel15MemoryAllocator9mem_allocEm                 # kernel::MemoryAllocator::mem_alloc(size_t)
.extern _ZN6kernel15MemoryAllocator8mem_freeEPv                 # kernel::MemoryAllocator::mem_free(void*)

.extern _ZN6kernel6thread13create_threadEPPNS0_3TCBEPFvPvES4_   # kernel::thread::create_thread(thread::TCB**, void (*)(void*), void*)
.extern _ZN6kernel6thread11exit_threadEv                        # kernel::thread::exit_thread()
.extern _ZN6kernel6thread9Scheduler8dispatchEv                  # kernel::thread::Scheduler::dispatch()

.extern _ZN7Console6handleEv                                    # Console::handle()

.extern _Z5errorv                                               # error()

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
    csrr t0, scause

    # ecall in user mode
    li t1, 8
    beq t0, t1, syscall_dispatch

    # ecall in system mode
    li t1, 9
    beq t0, t1, syscall_dispatch

    # timer
    li t1, 0x8000000000000001
    beq t0, t1, handle_timer

    # console
    li t1, 0x8000000000000009
    beq t0, t1, console

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

    li t0, 0x11
    beq a0, t0, thread_create

    li t0, 0x12
    beq a0, t0, thread_exit

    li t0, 0x13
    beq a0, t0, thread_dispatch

    j invalid_operator_code

mem_alloc:
    mv a0, a1   # size_t size
    call _ZN6kernel15MemoryAllocator9mem_allocEm
    j syscall_done

mem_free:
    mv a0, a1   # void* ptr
    call _ZN6kernel15MemoryAllocator8mem_freeEPv
    j syscall_done

thread_create:
    mv a0, a1   # PCB** handle
    mv a1, a2   # void (* body_)(void *)
    mv a2, a3   # void* arg
    call _ZN6kernel6thread13create_threadEPPNS0_3TCBEPFvPvES4_
    j syscall_done

thread_exit:
    call _ZN6kernel6thread11exit_threadEv
    j syscall_done

thread_dispatch:
    call _ZN6kernel6thread9Scheduler8dispatchEv
    j syscall_done

invalid_operator_code:
    li a0, -1

syscall_done:
    sd a0, 10 * 8(sp)
    j exit_trap

handle_timer:
    # 0b10 SSIP bit
    li t0, 0x2
    csrc sip, t0
    j exit_trap

console:
    call _ZN7Console6handleEv
    j exit_trap

illegal_instructions:
    call _Z5errorv

exit_trap:
    csrw sstatus, s1

    .irp index 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256

    sret
