.global _ZN6kernel6thread14switch_contextEPNS0_3TCBES2_         # kernel::thread::switch_context(kernel::thread::TCB*, kernel::thread::TCB*)

_ZN6kernel6thread14switch_contextEPNS0_3TCBES2_:
    sd ra, 24(a0)
    sd sp, 32(a0)

    ld ra, 24(a1)
    ld sp, 32(a1)

    ret
