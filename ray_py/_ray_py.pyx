#!python
#cython: language_level=3
#distutils: language = c++

from libcpp.vector cimport vector
from numpy cimport int64_t
import numpy as np

cdef extern from "src/testdata.h":
    void test()

def run_test():
    test()

# def simplex_noise(seed,x0,x1,nx,y0,y1,ny):
#     return np.array(simplex_noise_2d(seed,x0,x1,nx,y0,y1,ny))
#
