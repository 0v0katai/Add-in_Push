#ifndef _CASIOWIN_H
#define _CASIOWIN_H

#include <stdint.h>
#include <stddef.h>

/* List of syscalls whose addresses are known on Math+ OS versions.
   Each one is annotated with tested/confidence level for some OS versions.
   - todo:      Address not known
   - likely:    Confident/likely guess
   - used:      The function was used successfully at least once
   - (nothing): Tested, officially supported */

//=== Dynamic allocation =====================================================//

// v1 v2:likely
void *sys_malloc(size_t size);
// v1 v2:likely
void sys_free(void *ptr);
// v1 v2:likely
void *sys_realloc(void *ptr, size_t size);

//=== Drawing and display ====================================================//

// v1:likely v2:likely
void Bdisp_PutDisp_DD(void);
// v1 v2
void *GetVRAMAddress(void);
// v1 v2
void *Bdisp_AllClr_VRAM(void);
// v1:used v2:used
void PrintMini(int *x, int *y, char const *str_multibyte, int mode,
    int xlimit /* -1 */, int p6_zero, int p7_zero, int fgcolor, int bgcolor,
    int writeflag, int p11_zero); 
// v1:used v2:used
void PrintMiniMini(int *x, int *y, char const *str_multibyte, int mode1,
    int color3bit, int mode2);
// v1 v2:likely
void MMPrintRef(int *x, int *y, char const *str, int mode, int xmax,
    int margin_top, int margin_bottom, int fgcolor, int bgcolor, int p10_zero,
    int p11_zero);
// v1:likely v2:likely
void MMPrint(int x, int y, char const *str, int mode, int xmax,
    int margin_top, int margin_bottom, int fgcolor, int bgcolor, int p10_zero,
    int p11_zero);
// v1:used v2:used
void DrawFrame(int color);
// v1 v2:used
void MsgBoxPush(int size);
// v1 v2:used
void MsgBoxPop(void);
// v1 v2:used
void Box2(int size, int unknown);

//=== Miscellaneous ==========================================================//

// v1 v2:used
void OS_InnerWait_ms(int ms);
// v1 v2:likely
void SetQuitHandler(void (*callback)());
// v1 v2:used
void USB_Popup(void);

//=== Keyboard ===============================================================//

// v1 v2
int GetKey(int *key);
// v1:used v2:likely
int GetKeyWait(int *column, int *row, int wait_type, int timeout, int menu,
                  uint16_t *keycode);

#define KEY_CTRL_PREVTAB    30075
#define KEY_CTRL_UP         30018
#define KEY_CTRL_NEXTTAB    30074
#define KEY_CTRL_PGUP       30065
#define KEY_CTRL_SETUP      30037
#define KEY_CTRL_SETTINGS   KEY_CTRL_SETUP // reused from old API
#define KEY_CTRL_EXIT       30002
#define KEY_CTRL_BACK       KEY_CTRL_EXIT // reused from old API
#define KEY_CTRL_LEFT       30020
#define KEY_CTRL_EXE        30004
#define KEY_CTRL_OK         KEY_CTRL_EXE // cannot be distinguished from EXE
#define KEY_CTRL_RIGHT      30021
#define KEY_CTRL_PGDOWN     30066
#define KEY_CTRL_SHIFT      30006
#define KEY_CTRL_ALPHA      30007
#define KEY_CTRL_VARS       30016
#define KEY_CTRL_DOWN       30023
#define KEY_CTRL_CATALOG    30100
#define KEY_CTRL_TOOLS      30068
#define KEY_CTRL_FORMAT     30067
#define KEY_CTRL_MENU       30003
#define KEY_CTRL_AC         30015

#define KEY_CHAR_DP         0x2e
#define KEY_CHAR_EXP        0x0f
#define KEY_CHAR_PMINUS     0x87
#define KEY_CHAR_PLUS       0x89
#define KEY_CHAR_MINUS      0x99
#define KEY_CHAR_MULT       0xa9
#define KEY_CHAR_DIV        0xb9
#define KEY_CHAR_FRAC       0xbb
#define KEY_CHAR_LPAR       0x28
#define KEY_CHAR_RPAR       0x29
#define KEY_CHAR_COMMA      0x2c
#define KEY_CHAR_STORE      0x0e
#define KEY_CHAR_LOG        0x95
#define KEY_CHAR_LN         0x85
#define KEY_CHAR_SIN        0x81
#define KEY_CHAR_COS        0x82
#define KEY_CHAR_TAN        0x83
#define KEY_CHAR_SQUARE     0x8b
#define KEY_CHAR_POW        0xa8
#define KEY_CHAR_IMGNRY     0x7f50
#define KEY_CHAR_LIST       0x7f51
#define KEY_CHAR_MAT        0x7f40
#define KEY_CHAR_EQUAL      0x3d
#define KEY_CHAR_PI         0xd0
#define KEY_CHAR_ANS        0xc0
#define KEY_CHAR_LBRCKT     0x5b
#define KEY_CHAR_RBRCKT     0x5d
#define KEY_CHAR_LBRACE     0x7b
#define KEY_CHAR_RBRACE     0x7d
#define KEY_CHAR_CR         0x0d
#define KEY_CHAR_CUBEROOT   0x96
#define KEY_CHAR_RECIP      0x9b
#define KEY_CHAR_ANGLE      0x7f54
#define KEY_CHAR_EXPN10     0xb5
#define KEY_CHAR_EXPN       0xa5
#define KEY_CHAR_ASIN       0x91
#define KEY_CHAR_ACOS       0x92
#define KEY_CHAR_ATAN       0x93
#define KEY_CHAR_ROOT       0x86
#define KEY_CHAR_POWROOT    0xb8
#define KEY_CHAR_SPACE      0x20
#define KEY_CHAR_DQUATE     0x22
#define KEY_CHAR_VALR       0xcd
#define KEY_CHAR_THETA      0xce

//=== Filesystem access ======================================================//

#define BFile_File 1
#define BFile_Folder 5

#define BFile_ReadOnly        0x01
#define BFile_WriteOnly       0x02
#define BFile_ReadWrite       (BFile_ReadOnly | BFile_WriteOnly)
#define BFile_Share           0x80
#define BFile_ReadShare       (BFile_ReadOnly | BFile_Share)
#define BFile_ReadWriteShare  (BFile_ReadWrite | BFile_Share)

// v1:likely v2:likely
int BFile_Remove(uint16_t const *path);
// v1:likely v2:likely
int BFile_Rename(uint16_t const *oldpath, uint16_t const *newpath);
// v1:likely v2:likely
int BFile_Create(uint16_t const *path, int type, int *size);
// v1:used v2:used
int BFile_Open(uint16_t const *path, int mode);
// v1:used v2:used
int BFile_Close(int fd);
// v1:used v2:used
int BFile_Size(int fd);
// v1:likely v2:likely
int BFile_Write(int fd, void const *data, int even_size);
// v1:used v2:used
int BFile_Read(int handle, void *data, int size, int whence);
// v1:likely v2:likely
int BFile_Seek(int fd, int offset);
// v1:likely v2:likely
int BFile_GetPos(int fd);

struct BFile_FileInfo {
    uint16_t index;
    uint16_t type;
    uint32_t file_size;
    /* Data size (file size minus the header) */
    uint32_t data_size;
    /* Is 0 if the file is complete */
    uint32_t property;
    /* Address of first fragment (do not use directly) */
    void *address;
};

#define BFile_Type_Directory  0x0000
#define BFile_Type_File       0x0001
#define BFile_Type_Addin      0x0002
#define BFile_Type_Eact       0x0003
#define BFile_Type_Language   0x0004
#define BFile_Type_Bitmap     0x0005
#define BFile_Type_MainMem    0x0006
#define BFile_Type_Temp       0x0007
#define BFile_Type_Dot        0x0008
#define BFile_Type_DotDot     0x0009
#define BFile_Type_Volume     0x000a
#define BFile_Type_Archived   0x0041

// v1:used v2:used
int BFile_FindFirst(uint16_t const *pattern, int *shandle,
                       uint16_t *found, struct BFile_FileInfo *fileinfo);
// v1:used v2:used
int BFile_FindNext(int shandle, uint16_t *found,
                      struct BFile_FileInfo *fileinfo);
// v1:used v2:used
int BFile_FindClose(int shandle);

// v1:likely v2:likely
void BFile_NameToStr_ncpy(char *dst, uint16_t const *src, int len);
// v1:likely v2:likely
void BFile_StrToName_ncpy(uint16_t *dst, char const *src, int len);

//=== Time ===================================================================//

// v1:likely v2:likely
int Timer_Install(int id, void (*handler)(void), int delay);
// v1:likely v2:likely
int Timer_Deinstall(int id);
// v1:likely v2:likely
int Timer_Start(int id);
// v1:likely v2:likely
int Timer_Stop(int id);

//=== USB ====================================================================//

// v1 v2:used
int USB_Open(short param_1);
// v1 v2:likely
int USB_IsOpen(void);
// v1 v2:likely
int USB_Close(void);
// v1 v2:used
int USB_ForceClose(int one);

// v1 v2:used
int USB_Read(unsigned char *out, int sz, short *count);
// v1 v2:likely
int USB_ReadSingle(unsigned char *out);
// v1 v2:likely
int USB_Peek(int idx, unsigned char *out);
// v1 v2:used
int USB_PollRX(void);
// v1 v2:used
int USB_ClearRX(void);

// v1 v2:used
int USB_Write(const unsigned char *buf, int count);
// v1 v2:likely
int USB_WriteSingle(unsigned char x);
// v1 v2:likely
int USB_PollTX(void);
// v1 v2:likely
int USB_ClearTX(void);

#endif /* _CASIOWIN_H */
