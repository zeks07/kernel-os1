.global _ZN6thread14switch_contextEPNS_3TCBES1_                 # thread::switch_context(thread::TCB*, thread::TCB*)

.global _ZN6thread3TCB12save_contextEPNS_7Contextv              # thread::TCB::save_context()
.global _ZN6thread3TCB12load_contextEPNS_7Contextv              # thread::TCB::load_context()


_ZN6thread14switch_contextEPNS_3TCBES1_:
    sd ra, 24(a0)
    sd sp, 32(a0)

    ld ra, 24(a1)
    ld sp, 32(a1)

    ret

_ZN6thread3TCB12save_contextEPNS_7Contextv:
    sd ra, 24(a0)
    sd sp, 32(a0)
    ret

_ZN6thread3TCB12load_contextEPNS_7Contextv:
    ld ra, 24(a0)
    ld sp, 32(a0)
    ret
