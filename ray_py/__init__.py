from ._ray_py import dump_test_data,calc_test_depths,calc_depths

import numpy as np

def intrinsic_depth(r,tau0,index0,transition_tau,index1):
    r2d = np.sqrt(r[:,0]**2+r[:,1]**2);
    z = np.fabs(r[:,2])
    theta = np.arctan2(z,r2d)
    below_transition = (theta<transition_tau)
    depths = np.zeros(len(theta))
    depths[below_transition] = tau0-theta[below_transition]/index0
    depths[~below_transition] = tau0-transition_tau/index0 - (depths[~below_transition]-transition_tau)/index1
    return depths
