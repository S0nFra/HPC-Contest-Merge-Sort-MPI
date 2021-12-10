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