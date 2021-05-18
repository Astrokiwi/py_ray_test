import ray_py
from mpi4py import MPI
import numpy as np

if __name__=='__main__':
    depths = ray_py.calc_test_depths()

    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()

    if rank==0:
        # print(depths)
        check_depths = np.loadtxt("test/pos.out",usecols=3)
        assert np.allclose(depths,check_depths,atol=1.e-7,rtol=0)
        # print(check_depths)
    # ray_py.dump_test_data()

    r = np.random.random((100,3))
    h = np.random.random(100)
    op = np.ones(100)

    tau=ray_py.calc_depths(r,h,op)
    print(rank,tau)