#!python
#cython: language_level=3
#distutils: language = c++

from libcpp.vector cimport vector
# from numpy cimport int64_t
import numpy as np

cdef extern from "src/testdata.h":
    void run_test()
    vector[double] test_depths()

cdef extern from "src/runfrompy.h":
    vector[double] raytrace_from_arrays(vector[vector[double]] pos,vector[double] smoothing,vector[double] opacity)

def dump_test_data():
    run_test()

def calc_test_depths():
    return np.array(test_depths())

def calc_depths(pos,smoothing,opacity):
    assert(pos.shape[0]==smoothing.size)
    assert(pos.shape[0]==opacity.size)
    assert(pos.shape[1]==3)

    return np.array(raytrace_from_arrays(pos,smoothing,opacity))

# def simplex_noise(seed,x0,x1,nx,y0,y1,ny):
#     return np.array(simplex_noise_2d(seed,x0,x1,nx,y0,y1,ny))
#
