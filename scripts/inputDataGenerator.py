import numpy as np


if __name__ == "__main__":
    min = np.iinfo(np.int32).min
    max = np.iinfo(np.int32).max
    size = 2**24
    filename = "2_24"
    s = np.random.randint(min,max,size)
    s.astype('int32').tofile(filename) 