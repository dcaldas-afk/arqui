#
# Poxim-V source makefile
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

#------------------------------------------------------------------------------
# Toolchain parameters
#------------------------------------------------------------------------------
PREFIX           := $(PWD)/poxim-v/bin
COMPILER         := $(PREFIX)/riscv32-elf-gcc
COMPILER_PARAMS  := -T poxim-v.ld -mabi=ilp32 -march=rv32imzicsr -nostartfiles -nostdlib -Wall -O3 -g poxim-v.s
DEBUGGER         := $(PREFIX)/riscv32-elf-gdb
DEBUGGER_PARAMS  := -batch -x poxim-v.gdb
DOWNLOAD         := curl -O -L -s bruno.dcomp.ufs.br/aulas/arq/projeto
DOWNLOAD_FILES   := INFO OUTPUT TUTORIAL Makefile.toolchain poxim-v.gdb poxim-v.ld poxim-v.s
OBJCOPY          := $(PREFIX)/riscv32-elf-objcopy
OBJCOPY_PARAMS   := -j .text* -j .rodata* -j .data* -j .bss* -O verilog
SHELL            := /bin/bash
SIMULATOR        := $(PREFIX)/qemu-system-riscv32
SIMULATOR_PARAMS := -M virt -m 4M -icount shift=0,align=on -bios none -chardev file,id=terminal,path=qemu.terminal.out,input-path=qemu.terminal.in -display none -semihosting -serial chardev:terminal -S -s -d int
SOURCES          := $(wildcard 1_*.s 2_*.s 3_*.s)
#------------------------------------------------------------------------------
.SILENT:
#------------------------------------------------------------------------------
all:
	if ! [ -f INFO ]; then\
		$(DOWNLOAD)/INFO;\
	fi;\
	cat INFO;
	printf "Available options:\n";
	printf "%s build    : build all sources to ELF and hex files\n" "-";
	printf "%s download : download required source files\n" "-";
	printf "%s run      : simulate project using QEMU\n" "-";
	printf "%s clean    : remove generated files\n" "-";
	printf "\n";
#------------------------------------------------------------------------------
dependencies:
	compiler_missing=`type $(COMPILER) > /dev/null 2>&1; echo $$?`;\
	debugger_missing=`type $(DEBUGGER) > /dev/null 2>&1; echo $$?`;\
	objcopy_missing=`type $(OBJCOPY) > /dev/null 2>&1; echo $$?`;\
	simulator_missing=`type $(SIMULATOR) > /dev/null 2>&1; echo $$?`;\
	if [ $$compiler_missing -ne 0 ] || [ $$debugger_missing -ne 0 ] || [ $$objcopy_missing -ne 0 ] || [ $$simulator_missing -ne 0 ]; then printf "error: please check Poxim-V toolchain installation (make -f Makefile.toolchain)\n"; exit 1; fi;\
#------------------------------------------------------------------------------
%.elf: %.s poxim-v.s poxim-v.ld
	printf "%s\n" "--------------------------------------------------------------------------------";
	printf "$(@) <- $(<)\n";
	$(COMPILER) $(COMPILER_PARAMS) $(<) -o $(@)
	chmod -x $(@)
#------------------------------------------------------------------------------
%.hex: %.elf
	printf "$(@) <- $(<)\n";
	$(OBJCOPY) $(OBJCOPY_PARAMS) $(<) $(@)
	chmod -x $(@)
#------------------------------------------------------------------------------
build: dependencies $(foreach source,$(SOURCES), $(source:.s=.elf) $(source:.s=.hex))
	printf "%s\n" "--------------------------------------------------------------------------------";
#------------------------------------------------------------------------------
define retrieve
	$(DOWNLOAD)/$(1)
	printf "  > $(1)\n";
endef
#------------------------------------------------------------------------------
download:
	printf "%s\n" "--------------------------------------------------------------------------------";
	printf "> Downloading source files from server\n";
	$(foreach file, $(DOWNLOAD_FILES),\
		$(call retrieve,$(file))\
	)
	printf "%s\n" "--------------------------------------------------------------------------------";
#------------------------------------------------------------------------------
run: $(PROJECT).elf
	if [[ -n "$(PROJECT)" ]]; then \
		touch qemu.terminal.in;\
		($(SIMULATOR) $(SIMULATOR_PARAMS) -kernel $(PROJECT).elf | $(DEBUGGER) $(DEBUGGER_PARAMS) $(PROJECT).elf) 2>&1 | awk '{ if((!/(^\s*$$)/) && (!/.*Breakpoint.*/) && (!/sending:|received:/) && (!/.*\(.*\).*at.*/) && (!/^[0-9]+.*$$/)) { sub(/^=>\s/, "> ", $$0); sub("riscv_cpu_do_interrupt:", "  > trap @", $$0); print $$0; } }';\
	else \
		printf "usage: make run PROJECT=example\n";\
	fi;
#------------------------------------------------------------------------------
clean:
	rm -rf *.elf *.o
#------------------------------------------------------------------------------
