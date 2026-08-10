#include "mmu.h"
#include <stdint.h>
#include <stdbool.h>

void invalidateTLB(void)
{
    *MMUCR |= 0x00000004;
}

static bool MMU_IsEnabled(void)
{
    return *MMUCR & 1;
}

void MMU_SetEnabled(bool enabled)
{
    if (MMU_IsEnabled() == enabled)
        return;

    u32 setting;
    if (enabled)
        setting = (0 << 26) // LRUI = 0
                + ((63-9) << 18) // URB reserves 9 entries (8*64kB URAM + 1 NULL)
                + (0 << 10) // URC = 0
                + (1 << 8) // SV = 1
                + (0 << 2) // do not invalidate
                + (1 << 0); // enable MMU
    else
        setting = (0 << 26) // LRUI = 0
                + (63 << 18) // initial value
                + (0 << 10) // URC = 0
                + (1 << 8) // initial value
                + (0 << 2) // do not invalidate
                + (0 << 0); // disable MMU

    *MMUCR = setting;
    __asm__("icbi @%0":: "r"(0xa0000000));
}

void MMU_Map(void *virt, void *phys, int size, int URC)
{
    if ((u32)virt >= 0x80000000)
        return;

    int SZ;
    if (size == 1 << 10)
        SZ = 0;
    else if (size == 1 << 12)
        SZ = 1;
    else if (size == 1 << 16)
        SZ = 2;
    else if (size == 1 << 20)
        SZ = 3;
    else
        return;

    if (URC >= 0 && URC < 64) {
        *MMUCR = (*MMUCR & 0xffff03ff) | (URC << 10);
        __asm__("icbi @%0":: "r"(0xa0000000));
    }

    /* No ASID since we're in single-virtual-memory mode. */
    *PTEH = (u32)virt & ~(size - 1);

    union { u32 data; struct MMU_UTLB_Data bits; } entry;
    entry.data = (u32)phys & 0x1fffffff;
    entry.bits.V = 1;
    entry.bits.SZ1 = (SZ >> 1);
    entry.bits.SZ0 = (SZ & 1);
    entry.bits.PR = 3; // U:rw
    entry.bits.C = 1;
    entry.bits.D = 1;
    entry.bits.SH = 0;
    entry.bits.WT = 0;

    *PTEL = entry.data;
    __asm__("ldtlb; nop");
}