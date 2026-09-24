#
# Poxim-V linker script
# 
# (C) Copyright 2024 Bruno Otavio Piedade Prado
#
# This file is part of Poxim-V.
#
# Poxim-V is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Poxim-V is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Poxim-V.  If not, see <https://www.gnu.org/licenses/>.
#

OUTPUT_ARCH(riscv)
ENTRY(_start)

_RAM_origin = 0x80000000;
_RAM_length = 32K;

MEMORY {
    RAM(rwx) : ORIGIN = _RAM_origin, LENGTH = _RAM_length
}
PHDRS {
    ro PT_LOAD FLAGS(4);
    rx PT_LOAD FLAGS(5);
    rw PT_LOAD FLAGS(6);
}
SECTIONS {
    .text : ALIGN(4) {
        _start_text = .;
        *(.text.vector_table)
        *(.text.init)
        *(.text*)
        _end_text = .;
    } > RAM : rx
    .rodata : ALIGN(4) {
        _start_rodata = .;
        *(.rodata*)
        _end_rodata = .;
    } > RAM : ro
    .data : ALIGN(4) {
        _start_data = .;
        *(.data*)
        _end_data = .;
    } > RAM : rw
    .bss : ALIGN(4) {
        _start_bss = .;
        *(.bss*)
        *(COMMON)
        _end_bss = .;
    } > RAM : rw
    .heap (NOLOAD) : ALIGN(4) {
        _heap_pointer = .;
    } > RAM : rw
    .stack ORIGIN(RAM) + LENGTH(RAM) (NOLOAD) : ALIGN(16) {
        _stack_pointer = .;
    } > RAM : rw
}