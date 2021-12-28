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
add_executable(merge_serial_O0 src/mergesort_serial.c src/utils.c include/datatype.h include/utils.h include/mergesort_serial.h)

find_package(MPI REQUIRED)
if(MPI_C_FOUND)
    message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
    target_link_libraries(merge_mpi_O0 PUBLIC MPI::MPI_C)
endif()

target_compile_options(merge_mpi_O0 PRIVATE -O0)
target_compile_options(merge_serial_O0 PRIVATE -O0)
#-----------------------------------------------------------------------------

#------------------------------- DOCUMENTATION -------------------------------
find_package(Doxygen REQUIRED dot)

if(DOXYGEN_FOUND)
	set(DOXYGEN_GENERATE_XML YES)
	doxygen_add_docs(
	  docs 
		${CMAKE_SOURCE_DIR}/include/datatype.h 
		${CMAKE_SOURCE_DIR}/include/mergeMPI.h
		${CMAKE_SOURCE_DIR}/include/mergesort_serial.h
		${CMAKE_SOURCE_DIR}/include/utils.h
		${CMAKE_SOURCE_DIR}/src/mergeMPI.c
		${CMAKE_SOURCE_DIR}/src/mergesort_serial.c
		${CMAKE_SOURCE_DIR}/src/utils.c
		)
endif()
#-----------------------------------------------------------------------------

#----------------------------- MEASURE GENERATION ----------------------------
find_package(Python3 COMPONENTS Interpreter REQUIRED)
find_program(Pip pip REQUIRED)

add_custom_target(
	install_requirems
	COMMAND REQUIREMENTS_FILE=${CMAKE_CURRENT_SOURCE_DIR}/scripts/requirements.txt ${Pip} install -r ${CMAKE_CURRENT_SOURCE_DIR}/scripts/requirements.txt
	COMMENT "Installing python dependencies [${Pip} install]"
	DEPENDS	${CMAKE_CURRENT_SOURCE_DIR}/scripts/requirements.txt
)

add_custom_target(
    generate_measures
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_measures.py
    COMMENT "Generating measures [python ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_measures.py]"
    DEPENDS	
		install_requirems
		${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_measures.py
)
#-----------------------------------------------------------------------------

#----------------------------- MEASURE EXTRACTION ----------------------------
add_custom_target(
    extract_measures
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/scripts/extract_measures.py
    COMMENT "Generating measures [python ${CMAKE_CURRENT_SOURCE_DIR}/scripts/extract_measures.py]"
    DEPENDS	
		install_requirems
		${CMAKE_CURRENT_SOURCE_DIR}/scripts/extract_measures.py
)
#-----------------------------------------------------------------------------