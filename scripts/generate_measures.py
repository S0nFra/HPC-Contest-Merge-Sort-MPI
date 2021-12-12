import os
from pathlib import Path
import subprocess as sp
from tqdm import tqdm
from TestResult import TestResult
from math import log2

SRC_FOLDER = Path.cwd().parent / Path('build/executables')
DST_FOLDER = Path.cwd().parent / Path('measures')
INPUT_FILES_PATH = Path.cwd().parent / Path('data')

INPUT_SIZE_LIST = (2 ** 16, 2 ** 18, 2 ** 19, 2 ** 20)
PROCS = (0, 2, 4, 8, 16)  # 1 not considered
MSRS = 100  # Number of measures taken
VERSIONS = (0, 1, 2, 3)
CASES = 2  # O0 and O3

CASE_ONE_PATH = DST_FOLDER / Path("Case_1")
CASE_TWO_PATH = DST_FOLDER / Path("Case_2")


def create_dir_if_not_exists(dir_path: Path):
    if not Path.is_dir(dir_path):  # check if destination exists
        Path.mkdir(dir_path)  # create the folder if not exists


def get_files_in_dir(source_path: Path, with_sort=False):
    exe_list = os.listdir(source_path)
    if with_sort:
        return sorted(exe_list)
    return exe_list


def measure_exec_time(command: str,is_parallel:bool):
    """Call linux 'time' to get the execution times (real time, user time, kernel time) of the executable
       Return an object TestResult containing all relevant info about the execution times
    """
    p = sp.Popen(f'\'time\' -f \";%e;%U;%S\" {command}', shell=True, stderr=sp.STDOUT, stdout=sp.PIPE)
    msg, _ = p.communicate()
    return TestResult.convert_to_data(msg,is_parallel)


def generate_measures():
    create_dir_if_not_exists(DST_FOLDER)
    create_dir_if_not_exists(CASE_ONE_PATH)
    create_dir_if_not_exists(CASE_TWO_PATH)

    executables = get_files_in_dir(SRC_FOLDER, with_sort=True)
    inputs = get_files_in_dir(INPUT_FILES_PATH)

    mpi_executables = executables[:CASES]  # first half CASES are mpi execs
    serial_executables = executables[CASES:]  # second half CASES are serial execs

    for case in range(CASES):  # for each case
        if case == 0:
            case_path = CASE_ONE_PATH
        else:
            case_path = CASE_TWO_PATH

        exe_serial_path = SRC_FOLDER / Path(serial_executables[case])
        exe_mpi_path = SRC_FOLDER / Path(mpi_executables[case])

        for version in VERSIONS:  # for each version
            version_path = case_path / Path(f"version_{version}")
            create_dir_if_not_exists(version_path)
            for in_size_n, input_size in enumerate(INPUT_SIZE_LIST):  # for each size

                input_file = INPUT_FILES_PATH / Path(inputs[in_size_n])
                in_size = f"{int(log2(input_size))}"

                size_out_path = version_path / Path(f"size_{in_size}")
                create_dir_if_not_exists(size_out_path)

                for proc_num in PROCS:  # for each number of Computing Elements

                    if proc_num == 0:  # serial
                        output_measures_path = size_out_path / Path(f"serial_{in_size}.csv")
                        command = f"{exe_serial_path} \'{input_file}\' {input_size}"
                    else:  # parallel
                        output_measures_path = size_out_path / Path(f"mpi_{proc_num}_{in_size}.csv")
                        command = f"mpirun -np {proc_num} {exe_mpi_path} \'{input_file}\' {input_size} {version}"

                    # print(f"executing version {version}, input size {input_size} and {proc_num} CE with command: {
                    # command}") print(output_measures_path)

                    with open(Path(output_measures_path), 'w+') as fout:
                        if proc_num == 0:
                            fout.write('size;processes;read_time;merge_time;elapsed;user;sys\n')
                            desc = f"Executing {exe_serial_path.name} version {version} with size 2^{in_size}..."
                        else:
                            fout.write('size;processes;read_time;local_sort_time;merge_time;elapsed;user;sys\n')
                            desc = f"Executing {exe_mpi_path.name} version {version} with {proc_num} processes and " \
                                   f"size 2^{in_size} ... "
                        # helpful progress bar
                        for _ in tqdm(range(MSRS), desc=desc):
                            data = measure_exec_time(command, proc_num != 0) # changes if serial or parallel according to the proc_num value
                            # writing into the file the repr of TestResult
                            fout.write(str(data))


if __name__ == "__main__":
    generate_measures()
