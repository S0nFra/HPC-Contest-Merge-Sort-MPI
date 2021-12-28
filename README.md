# HPC Contest MPI - Merge Sort

## Description

Parallelize and Evaluate Performances of Merge Sort Algorithms by using MPICH.

## Prerequisites

- A [UNIX ](https://en.wikipedia.org/wiki/Unix) machine. **Vanilla Windows is not supported**, however [WSL](https://docs.microsoft.com/en-us/windows/wsl/install) can be a good alternative.

- [MPICH](https://www.mpich.org/), version **3.3.2** or greater

- [CMake](https://cmake.org/download/), version **3.16.3** or greater

- [Python](https://www.python.org/downloads/), version **3.8** or greater

- [Python Pip](https://pypi.org/project/pip/), version **20.0.2** or greater

- [Doxygen](https://www.doxygen.nl/download.html) and Graphviz (dot) with the command

  ```bash
  sudo (apt,yum,dnf,...) install doxygen graphviz
  ```

  

## Install instruction

After cloning the repository, open a shell and create in the root path a folder named **build** with command 

```bash
mkdir build
```

Then enter the folder with

```bash
cd build
```

and execute the command

```bash
cmake ..
```

to generate the CMake files. When CMake has successfully ended, execute the command

```bash
make
```

to generate all the executable files needed; all the generated files will be inserted in the directory /build/executables.

Start the measure process by running the following code:

```bash
make generate_measures
```

## ⚠Please note⚠

The measures could take a **LOT** of time to execute depending on the machine on which it is run. Measures taken on the authors' PC are provided, so 

To extract all the infos from the measures, the command 

```bash
make extract_measures
```

is required to generate the speedup plots and the summary charts.

You can find our measures in */our_measure* folder.

### Optional 

The command 

```bash
doxygen Doxyfile.docs
```

in the *build* directory will generate the documentation of the c code. 

Once done, the generated documentation can be checked using a web browser and navigating to **build/html/index.html** page.