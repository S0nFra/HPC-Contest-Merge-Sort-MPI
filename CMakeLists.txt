
cmake_minimum_required(VERSION 3.12)

project(MergeSortProjectMPI
	DESCRIPTION "MergeSort project with MPICH"
	VERSION 0.1
	LANGUAGES C
	)

set(CMAKE_C_STANDARD 11) # default standard C11
enable_testing()

#------------------------- EXECUTABLE GENERATION ----------------------------
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/executables) #redirect executables in "executables" directory

include_directories(include)
add_executable(merge_mpi_O0 src/mergeMPI.c src/utils.c include/datatype.h include/utils.h include/mergeMPI.h)
add_executable(merge_mpi_O3 src/mergeMPI.c src/utils.c include/datatype.h include/utils.h include/mergeMPI.h)
add_executable(merge_serial_O0 src/mergesort_serial.c src/utils.c include/datatype.h include/utils.h include/mergesort_serial.h)
add_executable(merge_serial_O3 src/mergesort_serial.c src/utils.c include/datatype.h include/utils.h include/mergesort_serial.h)

find_package(MPI REQUIRED)
if(MPI_C_FOUND)
    message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
    target_link_libraries(merge_mpi_O0 PUBLIC MPI::MPI_C)
	target_link_libraries(merge_mpi_O3 PUBLIC MPI::MPI_C)
endif()

target_compile_options(merge_mpi_O0 PRIVATE -O0)
target_compile_options(merge_mpi_O3 PRIVATE -O3)
target_compile_options(merge_serial_O0 PRIVATE -O0)
target_compile_options(merge_serial_O3 PRIVATE -O3)
#-----------------------------------------------------------------------------

#------------------------------- DOCUMENTATION -------------------------------

#-----------------------------------------------------------------------------

#----------------------------- MEASURE GENERATION ----------------------------

#-----------------------------------------------------------------------------

#----------------------------- MEASURE EXTRACTION ----------------------------

#-----------------------------------------------------------------------------