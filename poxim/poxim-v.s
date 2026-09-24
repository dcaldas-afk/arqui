#
# Poxim-V initialization code
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

# Disable compressed instructions (16 bits)
.option norvc

# Text vector table subsection
.section .text.vector_table
.global _trap_entry
_start_entry:
    j _start
_trap_entry:
    j exception_handler
    j _trap_error
    j _trap_error
    j software_interruption_handler
    j _trap_error
    j _trap_error
    j _trap_error
    j timer_interruption_handler
    j _trap_error
    j _trap_error
    j _trap_error
    j external_interruption_handler
.align 2

# Setting weak aliases to trap handlers
.weak exception_handler
.set  exception_handler, _trap_error
.weak software_interruption_handler
.set  software_interruption_handler, _trap_error
.weak timer_interruption_handler
.set  timer_interruption_handler, _trap_error
.weak external_interruption_handler
.set  external_interruption_handler, _trap_error

# Text init subsection
.section .text.init
# bss initialization function
_init_bss:
    # Retrieving bss boundaries
    la a0, _start_bss
    la a1, _end_bss
    # Initializing with zero
    _init_bss_loop:
        # Writing zero to memory
        sw zero, 0(a0)
        # Incrementing pointer by 4
        addi a0, a0, 4
        # Checking if index < _end_bss
        blt a0, a1, _init_bss_loop
    # Returning from call
    ret

# Exit function
_exit:
    # Allocating 16 bytes on stack
    addi sp, sp, -16
    # Save exit parameter (sp[1] = code)
    sw a0, 4(sp)
    # Setting exit semihosting first argument
    li a0, 0x20
    # Setting exit extended code (sp[0] = 0x20026)
    li a1, 0x20026
    sw a1, 0(sp)
    # Setting exit semihost second argument
    add a1, zero, sp
    # Calling semihost function
    call _semihost
    # Deallocating 16 bytes on stack
    addi sp, sp, 16
    # Forever loop (should not execute)
    j .

# Trap error handler
.global _trap_error
_trap_error:
    # Setting error code
    li a0, 1
    # Halting simulator
    j _exit

# Semihost function
.balign 16
.global _semihost
_semihost:
    # Simulator semihost request
    slli zero, zero, 0x1f
    ebreak
    srai zero, zero, 7
    # Returning from call
    ret

# Start function
.global _start
_start:
    # Setting up stack register (16 bytes alignment)
    la sp, _stack_pointer
    # Initializing bss section
    call _init_bss
    # Calling main function
    call main
    # Halting simulator
    j _exit
