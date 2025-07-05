.global _ZN6kernel6thread14switch_contextEPNS0_3TCBES2_         # kernel::thread::switch_context(kernel::thread::TCB*, kernel::thread::TCB*)

.global _ZN6kernel6thread3TCB12save_contextEPNS_7Contextv       # kernel::thread::TCB::save_context()
.global _ZN6kernel6thread3TCB12load_contextEPNS_7Contextv       # kernel::thread::TCB::load_context()


_ZN6kernel6thread14switch_contextEPNS0_3TCBES2_:
    sd ra, 24(a0)
    sd sp, 32(a0)

    ld ra, 24(a1)
    ld sp, 32(a1)

    ret

_ZN6kernel6thread3TCB12save_contextEPNS_7Contextv:
    sd ra, 24(a0)
    sd sp, 32(a0)
    ret

_ZN6kernel6thread3TCB12load_contextEPNS_7Contextv:
    ld ra, 24(a0)
    ld sp, 32(a0)
    ret
