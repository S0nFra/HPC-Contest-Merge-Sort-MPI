import os
from pathlib import Path
import subprocess as sp
from tqdm import tqdm
from TestResult import TestResult
from natural_sorting import *

SRC_FOLDER = Path.cwd().parent / Path('build/executables')
DST_FOLDER = Path.cwd().parent / Path('measures')
INPUT_FILES_PATH = Path.cwd().parent / Path('data')
VERSION_ONE_PATH = DST_FOLDER / Path("Version 1")
VERSION_TWO_PATH = DST_FOLDER / Path("Version 2")

INPUT_SIZE_LIST = (10_000, 50_000, 100_000, 500_000)
PROCS = (0, 1, 2, 4, 8, 16, 32)
OPTIMS = (0, 1, 2)
MSRS = 200  # Number of measures taken

def create_dir_if_not_exists(dirpath:Path):
    if not os.path.isdir(dirpath):  # check if destination exists
        os.mkdir(dirpath)  # create the folder if not exists

def get_files_in_dir(source_path:Path,with_sort=False):
    exe_list = os.listdir(source_path)
    if(with_sort):
        return sorted(exe_list)
    return exe_list

def measure_exec_time(command : str) -> TestResult:
    """Call linux 'time' to get the execution times (real time, user time, kernel time) of the executable
       Return an object TestResult containing all relevant info about the execution times
    """
    p = sp.Popen(f'time -f \";%e;%U;%S\" {command}',shell = True, stderr = sp.STDOUT, stdout = sp.PIPE)
    msg, _ = p.communicate()
    return TestResult.convertToData(msg)

def generate_measures():
    create_dir_if_not_exists(DST_FOLDER)
    
    executables = get_files_in_dir(SRC_FOLDER,with_sort=True)
    inputs = get_files_in_dir(INPUT_FILES_PATH)
    
    if(inputs and len(inputs) == len(INPUT_SIZE_LIST)):
        inputs = sorted(inputs,key=natural_keys) #sort for increasing input size
    else:
        raise Exception("cannot load valid input files")
    

    for version in range(1,3): # two versions
        for i_size,size in enumerate(INPUT_SIZE_LIST):
                if(version == 1):
                    outdir_path = VERSION_ONE_PATH / Path(f"size_{size}")
                else:
                    outdir_path = VERSION_TWO_PATH / Path(f"size_{size}")
                    
                create_dir_if_not_exists(outdir_path)            
                
                for i_processes,proc_num in enumerate(PROCS):
                    
                    input_path = INPUT_FILES_PATH / Path(inputs[i_size])
                    
                    if(proc_num == 0): # serial 
                        OUT_FILE = outdir_path / Path(f"size_{size}_serial.csv")
                        source_exe_path = SRC_FOLDER / Path(serial_execs)
                        params = f"\'{input_path}\' {size} 0"
                    else: # with mpi
                        OUT_FILE = outdir_path / Path(f"size_{size}_thr_{proc_num}_paral.csv")
                        mpiMode = f"mpirun -np {proc_num}"
                        source_exe_path = SRC_FOLDER / Path(parall_execs)
                        params = f"\'{input_path}\' {size} {version}"
                        command = f"{mpiMode} \'{str(source_exe_path)}\' {params}"
                    
                    #print(f"out file: {OUT_FILE}\nsource: {source_exe_path}\nparameters: {params}\n")

                    #GENERATE 
                    with open(Path(OUT_FILE), 'w+') as fout:
                        #prima colonna da scrivere 
                        fout.write('size;nodes;read_time;merge_time;elapsed;user;sys\n')
                        if(proc_num == 0):
                            desc = f"Executing {source_exe_path.name} with size {size}..."
                        else:
                            desc = f"Executing {source_exe_path.name} with {proc_num} processes and size {size} ..."
                        # helpful progress bar
                        for i in tqdm(range(MSRS), desc=desc):
                            data = measure_exec_time(command)
                            # writing into the file the repr of TestResult
                            fout.write(str(data))

if __name__ == "__main__":
    generate_measures()