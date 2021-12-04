
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
add_executable(mpi_exe src/mergeMPI.c include/mergeMPI.h)

find_package(MPI REQUIRED)
if(MPI_C_FOUND)
    message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
    target_link_libraries(mpi_exe PUBLIC MPI::MPI_C)
endif()

target_compile_options(mpi_exe PRIVATE -O3)
#-----------------------------------------------------------------------------

#------------------------------- DOCUMENTATION -------------------------------

#-----------------------------------------------------------------------------

#----------------------------- MEASURE GENERATION ----------------------------

#-----------------------------------------------------------------------------

#----------------------------- MEASURE EXTRACTION ----------------------------

#-----------------------------------------------------------------------------