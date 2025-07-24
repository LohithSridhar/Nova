ENTRY(loader)

SECTIONS
{
  . = 0x00100000;                  /* Load address at 1MB */

  .gdt ALIGN(8) : {
    *(.gdt)
  }

  .realmode ALIGN(16) : {
    *(.realmode*)
  }

  .text ALIGN(16) : {
    *(.text)
  }

  .rodata ALIGN(16) : {
    *(.rodata)
  }

  .data ALIGN(16) : {
    *(.data)
  }

  .bss ALIGN(16) (NOLOAD) : {
    *(COMMON)
    *(.bss)
  }
}