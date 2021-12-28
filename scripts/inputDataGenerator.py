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
import numpy as np

sizes = [16,18,19,20] # exponent of the size
min = np.iinfo(np.int32).min
max = np.iinfo(np.int32).max

if __name__ == "__main__":
    
    for size in sizes:
        filename = f"2_{size}"
        # create a vector of 2^(size) elements 
        # with random integers in the range [INT_MIN,INT_MAX]
        s = np.random.randint(min,max,2**size) 
        s.astype('int32').tofile(filename) 