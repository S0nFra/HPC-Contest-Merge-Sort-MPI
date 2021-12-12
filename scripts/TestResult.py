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
            return f"{self.size_arr};{self.thread_num};{self.read_t};{self.compute};{self.real_time};{self.user_time};{self.sys_time}\n"
        else:
            return f"{self.size_arr};{self.thread_num};{self.read_t};{self.local_sort_time};{self.compute};{self.real_time};{self.user_time};{self.sys_time}\n"
