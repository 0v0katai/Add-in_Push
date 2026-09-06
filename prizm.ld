OUTPUT_FORMAT(elf32-sh)
OUTPUT_ARCH(sh4)
ENTRY(relocate)

MEMORY
{
    ram (rwx) : o = 0x8c7f0000, l = 64k
}

SECTIONS
{
    .text : {
        *(.relocate)
        *(.pretext)
        *(.text .text.*)
    } > ram

    .rodata : SUBALIGN(4) {
        *(.rodata)
        *(.rodata.*)
    } > ram

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
    } > ram

    _reloc_target = ORIGIN(ram) ;
    _reloc_size32 = ABSOLUTE(ALIGN(32) - ORIGIN(ram)) / 32 ;
}
