import numpy as np


if __name__ == "__main__":
    min = 0
    max = 1024
    size = 128_000
    filename = "big_giovanni_rnd"
    s = np.random.randint(min,max,size)
    s.astype('int32').tofile(filename) 