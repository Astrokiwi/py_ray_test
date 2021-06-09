#!python
#cython: language_level=3
#distutils: language = c++

from libcpp.vector cimport vector
# from numpy cimport int64_t
import numpy as np
from mpi4py import MPI

cdef extern from "src/testdata.h":
    void run_test()
    vector[double] test_depths()

cdef extern from "src/runfrompy.h":
    vector[double] raytrace_from_arrays(vector[vector[double]] pos,vector[double] smoothing,vector[double] opacity,vector[double] r_agn,double mass)

def dump_test_data():
    run_test()

def calc_test_depths():
    return np.array(test_depths())

def calc_depths(pos,smoothing,opacity,r_agn=None,mass=1.e-14):
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    if rank==0:
        assert(pos.shape[0]==smoothing.size)
        assert(pos.shape[0]==opacity.size)
        assert(pos.shape[1]==3)
    else:
        pos = np.zeros((1,1))
        smoothing = np.zeros(1)
        opacity = np.zeros(1)

    if r_agn is None:
        r_agn = np.zeros(3)
    else:
        assert(r_agn.size==3)
    print(rank,"Packaged in Python, sending")
    return np.array(raytrace_from_arrays(pos,smoothing,opacity,r_agn,mass))

# def simplex_noise(seed,x0,x1,nx,y0,y1,ny):
#     return np.array(simplex_noise_2d(seed,x0,x1,nx,y0,y1,ny))
#
