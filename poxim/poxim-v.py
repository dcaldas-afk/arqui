#!/usr/bin/env python3
#
# Poxim-V output checker
# 
# (C) Copyright 2026 Bruno Otavio Piedade Prado
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

# Command line argument parsing
import argparse
# Command line arguments
import sys

# Poxim-V version 1 constraints
POXIMV1_N_INSTRUCTION = 5
POXIMV1_N_OPERAND = 8
POXIMV1_N_REGISTER = 13
# Poxim-V version 1 dictionary
POXIMV1 = {
    "lb": {
        "address": set(),
        "operand": set()
    },
    "lh": {
        "address": set(),
        "operand": set()
    },
    "lw": {
        "address": set(),
        "operand": set()
    },
    "lbu": {
        "address": set(),
        "operand": set()
    },
    "lhu": {
        "address": set(),
        "operand": set()
    },
    "sb": {
        "address": set(),
        "operand": set()
    },
    "sh": {
        "address": set(),
        "operand": set()
    },
    "sw": {
        "address": set(),
        "operand": set()
    },
    "slli": {
        "address": set(),
        "operand": set()
    },
    "srli": {
        "address": set(),
        "operand": set()
    },
    "srai": {
        "address": set(),
        "operand": set()
    },
    "sll": {
        "address": set(),
        "operand": set()
    },
    "srl": {
        "address": set(),
        "operand": set()
    },
    "sra": {
        "address": set(),
        "operand": set()
    },
    "slt": {
        "address": set(),
        "operand": set()
    },
    "sltu": {
        "address": set(),
        "operand": set()
    },
    "add": {
        "address": set(),
        "operand": set()
    },
    "sub": {
        "address": set(),
        "operand": set()
    },
    "and": {
        "address": set(),
        "operand": set()
    },
    "or": {
        "address": set(),
        "operand": set()
    },
    "xor": {
        "address": set(),
        "operand": set()
    },
    "mul": {
        "address": set(),
        "operand": set()
    },
    "mulh": {
        "address": set(),
        "operand": set()
    },
    "mulhsu": {
        "address": set(),
        "operand": set()
    },
    "mulhu": {
        "address": set(),
        "operand": set()
    },
    "div": {
        "address": set(),
        "operand": set()
    },
    "divu": {
        "address": set(),
        "operand": set()
    },
    "rem": {
        "address": set(),
        "operand": set()
    },
    "remu": {
        "address": set(),
        "operand": set()
    },
    "auipc": {
        "address": set(),
        "operand": set()
    },
    "lui": {
        "address": set(),
        "operand": set()
    },
    "addi": {
        "address": set(),
        "operand": set()
    },
    "andi": {
        "address": set(),
        "operand": set()
    },
    "slti": {
        "address": set(),
        "operand": set()
    },
    "sltiu": {
        "address": set(),
        "operand": set()
    },
    "ori": {
        "address": set(),
        "operand": set()
    },
    "xori": {
        "address": set(),
        "operand": set()
    },
    "beq": {
        "address": set(),
        "operand": set()
    },
    "bne": {
        "address": set(),
        "operand": set()
    },
    "blt": {
        "address": set(),
        "operand": set()
    },
    "bge": {
        "address": set(),
        "operand": set()
    },
    "bltu": {
        "address": set(),
        "operand": set()
    },
    "bgeu": {
        "address": set(),
        "operand": set()
    },
    "jalr": {
        "address": set(),
        "operand": set()
    },
    "jal": {
        "address": set(),
        "operand": set()
    },
    "register": {
        "zero": 0,
        "ra": 0,
        "sp": 0,
        "gp": 0,
        "tp": 0,
        "t0": 0,
        "t1": 0,
        "t2": 0,
        "s0": 0,
        "s1": 0,
        "a0": 0,
        "a1": 0,
        "a2": 0,
        "a3": 0,
        "a4": 0,
        "a5": 0,
        "a6": 0,
        "a7": 0,
        "s2": 0,
        "s3": 0,
        "s4": 0,
        "s5": 0,
        "s6": 0,
        "s7": 0,
        "s8": 0,
        "s9": 0,
        "s10": 0,
        "s11": 0,
        "t3": 0,
        "t4": 0,
        "t5": 0,
        "t6": 0
    }
}
# Poxim-V version 1 checker
def poximv1(data):
    # Error status
    error = False
    # Iterating over instruction from output
    for instruction in data:
        # Splitting by space
        all = instruction.split()
        # Considering only instruction + operand format
        if len(all) > 1:
            # Retrieving data
            address_mnemonic, operands = all[0], all[1]
            address, mnemonic = address_mnemonic.split(":")
            operand = operands.split(",")
            # Adding instruction address tp set
            POXIMV1[mnemonic]["address"].add(address)
            # Iterating over operands
            for value in operand:
                # Adding operand value to set
                POXIMV1[mnemonic]["operand"].add(value)
                # Checking if operand is a register
                if value in POXIMV1["register"]:
                    # Incrementing register counter
                    POXIMV1["register"][value] += 1
    # Iterating over mnemonics excluding register entry
    for mnemonic in { mnemonic: data for mnemonic, data in POXIMV1.items() if mnemonic != "register" }:
        # Retrieving status
        status = (len(POXIMV1[mnemonic]["address"]) >= POXIMV1_N_INSTRUCTION) and (len(POXIMV1[mnemonic]["operand"]) >= POXIMV1_N_OPERAND)
        # Outputting information
        print(("   OK: " if status else "ERROR: ") + mnemonic + "[counter=" + str(len(POXIMV1[mnemonic]["address"])) + ",#operand=" + str(len(POXIMV1[mnemonic]["operand"])) + "]")
        # Updating error status
        error = error or status
    # Iterating over registers
    for name in POXIMV1["register"]:
        # Retrieving status
        status = (POXIMV1["register"][name] >= POXIMV1_N_REGISTER)
        # Outputting information
        print(("   OK: " if status else "ERROR: ") + name + "[counter=" + str(POXIMV1["register"][name]) + "]")
        # Updating error status
        error = error or status
    # Returning error status
    return error
# Poxim-V version 2 checker (TODO)
def poximv2(data):
    # Checking Poxim-V version 1 constraints
    error = poximv1(data)
    # Returning error status
    return error
# Poxim-V version 3 checker (TODO)
def poximv3(data):
    # Checking Poxim-V version 2 constraints
    error = poximv2(data)
    # Returning error status
    return error
# Main function    
def main(argv):
    # Error status
    error = False
    # Parsing command line arguments
    parser = argparse.ArgumentParser(description = "A Poxim-V output checker")
    group = parser.add_mutually_exclusive_group(required = True)
    group.add_argument("--poximv1", action="store_true", help = "Version 1")
    group.add_argument("--poximv2", action="store_true", help = "Version 2")
    group.add_argument("--poximv3", action="store_true", help = "Version 3")
    parser.add_argument("filename", help = "Output trace file name", type = str)
    args = parser.parse_args(argv[1:])
    # Reading output trace file
    data = open(args.filename, "r").readlines()
    # Poxim-V version 1
    if args.poximv1:
        # Calling checker
        error = poximv1(data)
    # Poxim-V version 2
    elif args.poximv2:
        # Calling checker
        error = poximv2(data)
    # Poxim-V version 3
    else:
        # Calling checker
        error = poximv3(data)
    # Exiting with error status
    exit(error)
# Main function entry
if __name__ == "__main__":
    # Calling main passing command line arguments
    main(sys.argv)