#include "casiowin.h"
#include "iokbd.h"
#include "mmu.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define dtext(x, y, str) \
    PrintMini(&(int){x}, &(int){y}, str, 0x42, -1, 0, 0, 0, -1, 1, 0)

void message(const char *str)
{
    Box2(1, 0);
    dtext(40, 96, str);
}

int read_bin(u8 *out, int sz)
{
    while (USB_PollRX() == 0) {
        OS_InnerWait_ms(25);
        if (SH7305_IOKBD_ROW(0) == 1)
            return -1;
    }
    short count = 0;
    int rc = USB_Read(out, sz, &count);
    (void)rc;
    return count;
}

int main(void)
{
    int key;

    message("Initiating Add-in Push...");
    Bdisp_PutDisp_DD();

    int status;
    do {
        status = USB_Open(0x20);
        if (status == 10)
            goto cleanup;
    } while (status == 5);
    USB_ClearRX();

    /* https://git.planet-casio.com/Lephenixnoir/fxsdk/src/commit/be54e37/fxlink/modes/push.c#L80 */
    static const char handshake[] = "USB loader ready";
    USB_Write((u8 *)handshake, sizeof(handshake));

    size_t bin_size = 0;
    if (read_bin((u8 *)&bin_size, 4) == -1 || bin_size > 0x370000)
        goto cleanup;

    void *rom_phy = (void *)((0x8c7f0000 - bin_size) & -0x10000);
    void *ram_phy = (void *)0x8c160000;
    void *rom_virt = (void *)0x00300000;
    void *ram_virt = (void *)0x08100000;

    for (size_t pos = 0; pos < bin_size; pos += 0x100)
        if (read_bin((u8 *)((u32)rom_phy + pos), 0x100) == -1)
            goto cleanup;
    USB_ForceClose(1);

    invalidateTLB();
    MMU_SetEnabled(true);

    /* Map the entire range (why the heck not?!) */
    for (int i = 0; i < 55; i++) {
        MMU_Map(rom_virt + (i << 16), rom_phy + (i << 16),
                0x10000, i);
    }
    /* Map user RAM to OS child process stack */
    for (int i = 0; i < 8; i++) {
        MMU_Map(ram_virt + (i << 16), ram_phy + (i << 16),
                0x10000, 55 + i);
    }
    /* Map NULL to ROM for read-only access */
    MMU_Map(NULL, (void *)0xa0000000, 0x1000, 63);

    /* Reset URC to 0 to avoid erasing the protected region */
    *MMUCR &= 0xffff03ff;
    __asm__("icbi @%0":: "r"(0xa0000000));

    u32 volatile *CCR = (void *)0xff00001c;
    *CCR |= (1 << 11) // Instruction Cache Invalidate
          + (1 << 3); // Operand Cache Invalidate
    __asm__("icbi @%0":: "r"(0xa0000000));

    u32 loadinfo[] = {
        0x00000001, 0x00000001,
        0x00000010, 0x8c200000,
        0x00000011, (u32)rom_phy,
        0x00000000,
    };

    int (*code)(int sig, u32 *loadinfo) = (void *)0x00300000;
    int rc = code(0x4d504d30 /* 'MPM0' */, loadinfo);
    (void)rc;

  cleanup:
    USB_ForceClose(1);
    MMU_SetEnabled(false);
    message("Aborted! Press MENU to exit.");
    while(true)
        GetKey(&key);

    return 0;
}
