#include "casiowin.h"
#include <stdint.h>
#include <stdio.h>

#define ALIGN_4K(addr) (((addr) + 4095) & ~4095)

#define dtext(x, y, str) \
    PrintMini(&(int){x}, &(int){y}, str, 0x42, -1, 0, 0, 0, -1, 1, 0)

void message(const char *str)
{
    Box2(1, 0);
    dtext(40, 96, str);
}

void write_back_operand_cache(void *ptr, size_t size)
{
    for (size_t offset = 0; offset <= size; offset += 32)
        __asm__ volatile("ocbwb @%0" :: "r"(ptr + offset));
}

int read_from_usb(unsigned char *out, int sz)
{
    while (USB_PollRX() == 0)
        OS_InnerWait_ms(25);
    short count = 0;
    int rc = USB_Read(out, sz, &count);
    (void)rc;
    return count;
}

int main(void)
{
    /* Uncomment this line if linking with fxlibc and enabling LTO */
    // memcpy((void *)0x8C200000, (const void *)0x8C200000, 0);

    message("Initiating Add-in Push...");
    Bdisp_PutDisp_DD();
    while (USB_Open(0x20) == 5);

    USB_ClearRX();
    USB_Write((unsigned char *)"USB loader ready", 0x11);

    size_t incoming_bytes = 0;
    read_from_usb((unsigned char *)&incoming_bytes, 4);
    if (incoming_bytes > 0x200000) {
        USB_ForceClose(1);
        message("Input is too large!");
        GetKey(&(int){0});
        return 0;
    }

    for (size_t offset = 0; offset < incoming_bytes; offset += 0x100)
        read_from_usb((unsigned char *)(0x8c200000 + offset), 0x100);

    USB_ForceClose(1);

    write_back_operand_cache((void *)0x8c200000, incoming_bytes);

    uint32_t volatile *CCR = (void *)0xff00001c;
    *CCR |= (1 << 11) // Instruction Cache Invalidate
          + (1 << 3); // Operand Cache Invalidate
    __asm__("icbi @%0":: "r"(0xa0000000));

    uint32_t loadinfo[] = {
        0x00000001, 0x00000001,
        0x00000010, ALIGN_4K(0x8c200000 + incoming_bytes),
        0x00000011, 0x8c700000,
        0x00000000,
    };

    int (*code)(int sig, uint32_t *loadinfo) = (void *)0x8c200000;
    int rc = code(0x4d504d30 /* 'MPM0' */, loadinfo);
    (void)rc;
    
    return 0;
}
