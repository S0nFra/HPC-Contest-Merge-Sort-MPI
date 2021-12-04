import numpy as np


if __name__ == "__main__":
    min = 0
    max = 2048
    size = 1_000_000
    filename = "giga_giovanni_rnd"
    s = np.random.randint(min,max,size)
    s.astype('int32').tofile(filename) 