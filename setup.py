# Build with:
# python setup.py build_ext --inplace

import setuptools
from Cython.Build import cythonize
import numpy as np
import os

mpi_compile_args = os.popen("mpicc --showme:compile").read().strip().split(' ')
mpi_link_args    = os.popen("mpicc --showme:link").read().strip().split(' ')
print(mpi_compile_args)
print(mpi_link_args)

lib_args = ["-L/srv/djw1g16/ray_dev/sph_raytrace/lib","-lsph_raytrace"]
std_args = ["-std=c++11"]
inc_args = ["-I/srv/djw1g16/ray_dev/sph_raytrace/src"]


ext_modules=cythonize(
        setuptools.Extension("ray_py._ray_py",
                             [
                              # "ray_py/src/absorbtree.cpp",
                              #    "ray_py/src/agn_kernel.cpp",
                                 "ray_py/src/ArrayParticlePositionCoupler.cpp",
                                 # "ray_py/src/raytracing.cpp",
                                 "ray_py/src/testdata.cpp",
                                 "ray_py/_ray_py.pyx"],
                             language="c++",  # generate C++ code
                             compiler_directives={'language_level' : 3},
                             extra_compile_args=std_args+inc_args+mpi_compile_args,
                             extra_link_args=std_args+lib_args+mpi_link_args))

setuptools.setup(
    name='ray_py',
    version='0.1.0',
    description='test',
    author='David Williamson',
    author_email="david.john.williamson@gmail.com",
    #     package_dir = {'simplex_terrain/': ''},
    packages=["ray_py"],
    #     packages=[setuptools.find_packages()],
    ext_modules=ext_modules,
    include_dirs=[np.get_include()]

    )