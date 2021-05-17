compile:
	python setup.py build_ext --inplace

clean:
	rm -r build
	rm ray_py/_ray_py.cpython*.so