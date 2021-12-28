# Course: High Performance Computing 2021/2022
#
# Lecturer: Francesco Moscato    fmoscato@unisa.it
#
# Group:
# Mario Pellegrino    0622701671  m.pellegrino42@studenti.unisa.it
# Francesco Sonnessa   0622701672   f.sonnessa@studenti.unisa.it
#
# Copyright (C) 2021 - All Rights Reserved
#
# This file is part of Contest - OMP.
#
# Contest - OMP is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Contest - OMP is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Contest - OMP.  If not, see <http://www.gnu.org/licenses/>.
from dataclasses import dataclass
import math


@dataclass
class TestResult:
    """class used to represent a test on a problem
    size_arr: the dimension of the problem
    thread_num: the number of threads used in the computation (0 if serial)

    read_t: time to read the input file with data for the problem
    compute: time to execute the main computation of the problem

    real_time: elapsed real time to execute the program
    user_time: Total number of CPU-seconds that the process spent in user mode
    sys_time: Total number of CPU-seconds that the process spent in kernel mode
    """
    size_arr: int
    thread_num: int
    read_t: float
    compute: float
    local_sort_time: float

    real_time: float
    user_time: float
    sys_time: float

    def convert_to_data(msg: str,is_parallel:bool):
        msg = "{}".format(msg.decode("utf-8")).replace('\n', '').split(';')
        if not(len(msg) == 7 or len(msg) == 8 and is_parallel):
            raise Exception("could not convert measures to valid data")

        if(is_parallel):
            # compute_time (time to execute mergesort) = elapsed_time - read_time
            compute_time = round(float(msg[4]) - float(msg[2]),6) # max 6 decimal digits 
            return TestResult(
            size_arr=msg[0],
            thread_num=msg[1],
            read_t=msg[2],
            compute=compute_time,
            local_sort_time=msg[3],
            real_time=msg[4],
            user_time=msg[5],
            sys_time=msg[6],
            )
        else:
            return TestResult(
            size_arr=msg[0],
            thread_num=msg[1],
            read_t=msg[2],
            compute=msg[3],
            local_sort_time= float('nan'),
            real_time=msg[4],
            user_time=msg[5],
            sys_time=msg[6]
            )
    
    def __str__(self) -> str:
        if(math.isnan(float(self.local_sort_time))):
            return f"{self.size_arr};{self.thread_num};{self.read_t};0;{self.compute};{self.real_time};{self.user_time};{self.sys_time}\n"
        else:
            return f"{self.size_arr};{self.thread_num};{self.read_t};{self.local_sort_time};{self.compute};{self.real_time};{self.user_time};{self.sys_time}\n"
