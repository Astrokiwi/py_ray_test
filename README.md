This is kind of a work in progress, but shows how you can wrap the raytracing library and call it from Python.

See `examples/raytrace.ipynb` for a Jupyter notebook showing an example of usage.

Install the C library `sph_raytrace` first, from `https://github.com/Astrokiwi/sph_raytrace`.

Clone the repo

Modify `lib_args` and `inc_args` to point to where you installed the library. Also make sure you have `LD_LIBRARY_PATH` set correctly.

Then `pip install .` should work. If not, try `make` first.