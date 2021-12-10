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

from os import mkdir
import os.path
import sys
from pathlib import Path
from tqdm import tqdm
from colorama import Fore, Back, Style
import numpy as np
from scipy import stats
import seaborn as sns
import matplotlib.pyplot as plt
import csv
import pandas
import re #Regular Expression
from tabulate import tabulate

def atoi(text):
    return int(text) if text.isdigit() else text

def natural_keys(text):
    '''
    alist.sort(key=natural_keys) sorts in human order
    http://nedbatchelder.com/blog/200712/human_sorting.html
    (See Toothy's implementation in the comments)
    '''
    return [ atoi(c) for c in re.split(r'(\d+)', text) ]

def compute_speedup(t, tp, nthr):
    speedup = t/tp
    eff = t/(tp*float(nthr))
    return speedup, eff

def plot_graph(on_x, on_y, save : Path = None):
    _, axes = plt.subplots(figsize=(12, 8))
    axes.plot(on_x, on_y, 'ro-', label='Experimental')
    axes.plot(on_x, on_x, color='blue', label='Ideal')
    plt.style.use('seaborn-darkgrid')
    plt.autoscale(enable=True, axis='x', tight=True)
    plt.autoscale(enable=True, axis='y', tight=True)
    plt.legend()
    plt.xlabel("Processors")
    plt.ylabel("Speedup")
    if save != None:
        plt.savefig(save)
    plt.close()

def make_table(data, header = "", filename : Path = None, print_table : bool=True ,save : bool =False, img : bool = False):
    
    if (save or img) and not filename:
        raise Exception("No filename to save file")

    table = []
    if header and data:
        table.append(header)
        table.append(data)
    else:
        table = data
    
    if print_table:
        print(tabulate(table, tablefmt='fancy_grid'))
    
    if save:
        with open(filename,'w') as f:
            for row in table:
                csv_write = csv.writer(f, delimiter = ';')
                csv_write.writerow(row)
    
    if img:
        filename = Path(str(filename).replace('.csv','.txt'))
        with open(filename,'w') as f:
            f.write(tabulate(table, tablefmt='fancy_grid'))
    pass

def extract_csv(data : dict, fileCSV : Path, csv_header = None, delimiter = ';', info:int = 0):
    if csv_header is None:
        csv_header = list(data.keys())
    
    df = pandas.read_csv(fileCSV, usecols=csv_header, delimiter=delimiter)
    for col in csv_header:
        x_data = df[col]
        if info > 0: print(f'\n\n### {fileCSV} ###\n',Fore.YELLOW + f'{col}:', Style.RESET_ALL, f'\n{x_data}')
        mean, std = stats.norm.fit(x_data)
        # 68,3% = P{ μ − 1,00 σ < X < μ + 1,00 σ }
        x_data = df[ (df[col] < (mean + std)) & (df[col] > (mean - std)) ][col]
        result = mean if np.mean(x_data) is np.NaN else np.mean(x_data)

        if info > 1: print(f'{fileCSV.name} - col: {col}','>>> mean:',mean,'std:',std,'final:',(fileCSV.name,result))
        
        data[col].append((fileCSV.name,result))

def get_data(csv_header : list, directory : Path = None, file_re = "", serial_re:str = '', info:int=0) -> dict:
    directory = os.getcwd() if directory is None else directory
    data = {}   # 'data' mantain all collected data
    
    folders = sorted([f for f in os.listdir(directory)], key=natural_keys)
    # for folder in tqdm(folders,desc='Data extraction'):
    for folder in folders:
        current_folder = Path(f'{directory}/{folder}')
        files = sorted([f for f in os.listdir(current_folder) if f.endswith(".csv") and re.match(file_re,f)], key=natural_keys)

        if len(files) < 2: continue
              
        data_folder = {}   # 'data' mantain processed data from csv files in a folder
        for key in csv_header:
            data_folder[key] = []

        if info > 0: print(f'Opening {current_folder.name}')
        for file in files:
            if info > 0: print('\t| working on: {}'.format(file))
            extract_csv(data_folder, current_folder / file, csv_header=csv_header)

        if info > 1: print('\n\n### DATA_FOLDER ###\n',data_folder,'\n\n')
        data[folder] = data_folder
    return data

def splitNumbers(numbers:list, re_serial:str):
    serial = 0
    parallel = []            
    for num in numbers:
        if re.match(re_serial,num[0]):
            serial = num[1]
        else:
            parallel.append(num[1])
    return serial, parallel

def serialIndex(l:list, re_serial:str) -> int:
    for i in l:
        if re.match(re_serial,i[0]):
            return l.index(i)
    return -1

if __name__ == '__main__':
    base_dir = Path(os.getcwd()).parent
    
    WORKLOAD_EXP = (16,18,19,20)
    PROCS = (0, 2, 4, 8, 16)
    VERSIONS = (0,1,2,3)
    CASES = 2

    RE_CSV = "(mpi_[0-9]+|serial)_[0-9]+"
    RE_SEQ = "serial_[0-9]+"

    # Read a file with columns name in csv files
    output_data_info = "size;processes;read_time;merge_time;elapsed;user;sys".split(';')
    targetColumn = "merge_time" if len(sys.argv) < 2 else sys.argv[1]
    
    if targetColumn not in output_data_info:        
        print(targetColumn, "wrong target column")
        exit()
    
    measure_source = []
    for case in range(1,CASES+1):
        tmp = base_dir/'measures'/f'Case_{case}'
        for folder in os.listdir(tmp):
            measure_source.append(tmp/folder)

    for move_on in measure_source:
        # print(Fore.YELLOW + os.getcwd(),end='')
        if os.getcwd() != move_on:
            os.chdir(move_on)
            # print(' ->',move_on,end='')
        print(Fore.YELLOW + "Working on dir:", move_on, end='')
        print(Style.RESET_ALL)
        
        print("Data extraction... ",end="")
        data = get_data(output_data_info, file_re=RE_CSV, serial_re=RE_SEQ)
        print("DONE")
    
        print("# Generating graphs and tables")
        # for folder in tqdm(data.keys()):
        for folder in data.keys():        
            print('>>',folder)
            dir = move_on / folder
            d = data[folder]

            serial,parallel = splitNumbers(d[targetColumn], RE_SEQ)

            # Generating graph
            speedup = []
            eff = []
            for i in range(len(PROCS)):
                tmp_speedup, tmp_eff = compute_speedup(serial if i!=0 else 0, parallel[i-1] if i!=0 else 1, PROCS[i] if i!=0 else 1)
                speedup.append(tmp_speedup)
                eff.append(tmp_eff)
            # speedup = [1/s for s in speedup]
            plot_graph(PROCS, speedup, Path(dir/'{}_{}.jpg'.format(folder,targetColumn)))
            
            # Generating tables
            serial_index = serialIndex(d[targetColumn], RE_SEQ)
            table = []
            header_table = ['Version','Process','Read time','Merge time','User','Sys','Elapsed','Speedup','Efficiency']
            table.append(header_table)
            j = 0
            for procs in PROCS:
                if procs == 0:
                    row = ['Serial',1,'%.5f'%d['read_time'][j][1],'%.5f'%d['merge_time'][j][1],'%.5f'%d['user'][j][1],'%.5f'%d['sys'][j][1],'%.5f'%d['elapsed'][j][1],1,1]
                else:
                    row = ['Parallel',procs,'%.5f'%d['read_time'][j][1],'%.5f'%d['merge_time'][j][1],'%.5f'%d['user'][j][1],'%.5f'%d['sys'][j][1],'%.5f'%d['elapsed'][j][1],'%.5f'%speedup[j],'%.5f'%eff[j]]
                j += 1
                table.append(row)
            # print(tabulate(table, tablefmt='fancy_grid'))
        
            make_table(table, filename= dir/'{}_{}_{}.csv'.format(folder,'table',targetColumn), img=True, save=True, print_table=False)