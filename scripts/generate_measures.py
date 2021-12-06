import os
from pathlib import Path
import subprocess as sp
from tqdm import tqdm
from natural_sorting import *
from math import log2

SRC_FOLDER = Path.cwd().parent / Path('build/executables')
DST_FOLDER = Path.cwd().parent / Path('measures')
INPUT_FILES_PATH = Path.cwd().parent / Path('data')

INPUT_SIZE_LIST = (2**16,2**18,2**19)
PROCS = (0, 1, 2, 4, 8, 16)
MSRS = 10  # Number of measures taken
VERSIONS = (1,2,3,4)
CASES = 2 # O0 and O3

CASE_ONE_PATH = DST_FOLDER / Path("Case_1")
CASE_TWO_PATH = DST_FOLDER / Path("Case_2")

def create_dir_if_not_exists(dirpath:Path):
    if not Path.is_dir(dirpath):  # check if destination exists
        Path.mkdir(dirpath)  # create the folder if not exists

def get_files_in_dir(source_path:Path,with_sort=False):
    exe_list = os.listdir(source_path)
    if(with_sort):
        return sorted(exe_list)
    return exe_list

def measure_exec_time(command : str):
    """Call linux 'time' to get the execution times (real time, user time, kernel time) of the executable
       Return an object TestResult containing all relevant info about the execution times
    """
    p = sp.Popen(f'time -f \";%e;%U;%S\" {command}',shell = True, stderr = sp.STDOUT, stdout = sp.PIPE)
    msg, _ = p.communicate()
    return 

def generate_measures():
    create_dir_if_not_exists(DST_FOLDER)
    create_dir_if_not_exists(CASE_ONE_PATH)
    create_dir_if_not_exists(CASE_TWO_PATH)
    
    executables = get_files_in_dir(SRC_FOLDER,with_sort=True)
    inputs = get_files_in_dir(INPUT_FILES_PATH)
    
    mpi_executables = executables[:CASES] # first half CASES are mpi execs
    serial_executables = executables[CASES:] # second half CASES are serial execs
    
    for case in range(CASES): # for each case
        if(case == 0):
            CASE_PATH = CASE_ONE_PATH
        else:
            CASE_PATH = CASE_TWO_PATH
            
        exe_serial_path = Path(serial_executables[case]).resolve()
        exe_mpi_path = Path(mpi_executables[case]).resolve()
        
        for version in VERSIONS: # for each version
            VERSION_PATH = CASE_PATH / Path(f"version_{version}")
            create_dir_if_not_exists(VERSION_PATH)
            for in_size_n,input_size in enumerate(INPUT_SIZE_LIST): # for each size
                
                input_file = Path(inputs[in_size_n]).resolve()
                
                for proc_num in PROCS: # for each number of Computing Elements
                    in_size = f"{int(log2(input_size))}"
                    if(proc_num == 0): # serial
                        output_measures_path = VERSION_PATH / Path(f"serial_{in_size}.csv")
                        command = f".{exe_serial_path} \'{input_file}\' {input_size}"
                    else: # parallel
                        output_measures_path = VERSION_PATH / Path(f"mpi_{proc_num}_{in_size}.csv")
                        command = f"mpirun -np {proc_num} .{exe_mpi_path} \'{input_file}\' {input_size} {version}"
                    
                    #print(f"executing version {version}, input size {input_size} and {proc_num} CE with command: {command}")
                    #print(output_measures_path)
                    
    
    

    

    

if __name__ == "__main__":
    generate_measures()