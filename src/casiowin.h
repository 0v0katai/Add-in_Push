#ifndef _CASIOWIN_H
#define _CASIOWIN_H

#include <stdint.h>
#include <stddef.h>

void Bdisp_PutDisp_DD(void);
void PrintMini(int *x, int *y, char const *str_multibyte, int mode,
    int xlimit /* -1 */, int p6_zero, int p7_zero, int fgcolor, int bgcolor,
    int writeflag, int p11_zero); 
void Box2(int size, int unknown);
void OS_InnerWait_ms(int ms);
int GetKey(int *key);
int USB_Open(short param_1);
int USB_ForceClose(int one);
int USB_Read(unsigned char *out, int sz, short *count);
int USB_PollRX(void);
int USB_ClearRX(void);
int USB_Write(const unsigned char *buf, int count);

#endif /* _CASIOWIN_H */
