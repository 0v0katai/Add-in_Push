#ifndef _IOKBD_H
#define _IOKBD_H

#include <stdint.h>

typedef volatile struct {
    union {
        uint8_t rows[10];
        struct {
            uint8_t row1, row0;
            uint8_t row3, row2;
            uint8_t row5, row4;
            uint8_t row7, row6;
            uint8_t row9, row8;
        };
    };
} __attribute__((packed, aligned(2))) SH7305_iokbd_t;

#define SH7305_IOKBD (*((SH7305_iokbd_t *)0xa44b0000))
#define SH7305_IOKBD_ROW(row) (SH7305_IOKBD.rows[row ^ 1])

#endif /* _IOKBD_H */
