from dataclasses import dataclass


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

    real_time: float
    user_time: float
    sys_time: float

    def convert_to_data(msg: str):
        msg = "{}".format(msg.decode("utf-8")).replace('\n', '').split(';')
        if len(msg) != 7:
            raise Exception("could not convert measures to valid data")

        return TestResult(
            size_arr=msg[0],
            thread_num=msg[1],
            read_t=msg[2],
            compute=msg[3],
            real_time=msg[4],
            user_time=msg[5],
            sys_time=msg[6]
        )

    def get_real_time(self):
        return float(self.real_time)

    def get_composed_time(self):
        return round(float(self.read_t) + float(self.compute), 3)

    def __str__(self) -> str:
        return f"{self.size_arr};{self.thread_num};{self.read_t};{self.compute};{self.real_time};{self.user_time};{self.sys_time}\n"
