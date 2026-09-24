#
# Poxim-V GDB script
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

target remote :1234
maintenance packet Qqemu.sstep=0x1
break *0x80000000
continue
while($_inferior_thread_count > 0)
    if($pc >= 0x80000000 && $pc <= 0x80400000)
        printf " ---------------------------------------------------------------------- \n"
        printf "|   zero=0x%08x|   ra=0x%08x|   sp=0x%08x|    gp=0x%08x|\n", $zero, $ra, $sp, $gp
        printf "|     tp=0x%08x|   t0=0x%08x|   t1=0x%08x|    t2=0x%08x|\n", $tp, $t0, $t1, $t2
        printf "|     s0=0x%08x|   s1=0x%08x|   a0=0x%08x|    a1=0x%08x|\n", $s0, $s1, $a0, $a1
        printf "|     a2=0x%08x|   a3=0x%08x|   a4=0x%08x|    a5=0x%08x|\n", $a2, $a3, $a4, $a5
        printf "|     a6=0x%08x|   a7=0x%08x|   s2=0x%08x|    s3=0x%08x|\n", $a6, $a7, $s2, $s3
        printf "|     s4=0x%08x|   s5=0x%08x|   s6=0x%08x|    s7=0x%08x|\n", $s4, $s5, $s6, $s7
        printf "|     s8=0x%08x|   s9=0x%08x|  s10=0x%08x|   s11=0x%08x|\n", $s8, $s9, $s10, $s11
        printf "|     t3=0x%08x|   t4=0x%08x|   t5=0x%08x|    t6=0x%08x|\n", $t3, $t4, $t5, $t6
        printf " ---------------------------------------------------------------------- \n"
        printf "|     pc=0x%08x|mtvec=0x%08x| mepc=0x%08x|mcause=0x%08x|\n", $pc, $mtvec, $mepc, $mcause
        printf "|mstatus=0x%08x|  mie=0x%08x|mtval=0x%08x|   mip=0x%08x|\n", $mstatus, $mie, $mtval, $mip
        printf " ---------------------------------------------------------------------- \n"
        x/i $pc
    end
    stepi
end
printf " ---------------------------------------------------------------------- \n"
