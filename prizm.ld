OUTPUT_FORMAT(elf32-sh)
OUTPUT_ARCH(sh4)
ENTRY(relocate)

MEMORY
{
    rom (rx)    : o = 0x8c7f0000, l = 64k
    ram (rwx)   : o = 0x08100000, l = 512k
    ilram (rwx) : o = 0xe5200000, l = 4k
    xyram (rwx) : o = 0xe500e000, l = 16k
}

SECTIONS
{
    _reloc = ORIGIN(rom) ;
    _size = SIZEOF(.text) + SIZEOF(.rodata)
          + SIZEOF(.data) + SIZEOF(.ilram)
          + SIZEOF(.xyram) ;
    _lword = _size / 4 ;

    .text : {
        *(.relocate)
        *(.pretext)
        *(.text .text.*)
    } > rom

    .rodata : SUBALIGN(4) {
        *(.rodata)
        *(.rodata.*)
    } > rom

    .bss : ALIGN(4) {
        _bbss = . ;
        *(.bss .bss*)
        *(COMMON)
        _ebss = . ;
    } > ram

    .data : ALIGN(4) {
        _datald = LOADADDR(.data) ;
        _sdata = . ;
        *(.data .data.*)
        _edata = . ;
    } > ram AT> rom

    .ilram : ALIGN(4) {
        _ilramld = LOADADDR(.ilram) ;
        _silram = . ;
        *(.ilram .ilram.*)
        _eilram = . ;
    } > ilram AT> rom

    .xyram : ALIGN(4) {
        _xyramld = LOADADDR(.xyram) ;
        _sxyram = . ;
        *(.xyram .xyram.*)
        _exyram = . ;
    } > xyram AT> rom
}
