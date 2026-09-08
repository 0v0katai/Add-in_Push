#ifndef _MMU_H
#define _MMU_H

#include <stdint.h>
#include <stdbool.h>

typedef unsigned int uint;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define PACKED(N) __attribute__((packed, aligned(N)))
#define ALIGN_4K(addr) (((addr) + 4095) & ~4095)

#define PTEH  ((u32 volatile *)0xff000000)
#define PTEL  ((u32 volatile *)0xff000004)
#define MMUCR ((u32 volatile *)0xff000010)

struct MMU_UTLB_Data {
    uint        :3;
    uint PPN    :19;
    uint        :1;
    uint V      :1;
    uint SZ1    :1;
    uint PR     :2;
    uint SZ0    :1;
    uint C      :1;
    uint D      :1;
    uint SH     :1;
    uint WT     :1;
} PACKED(4);

void invalidateTLB(void);
void MMU_SetEnabled(bool enabled);
void MMU_Map(void *virt, void *phys, int size, int URC);

#endif /* _MMU_H */