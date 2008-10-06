from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext



ext_modules = [Extension(
        "libreccage",                 # name of extension
        ["pDataSet.pyx", "DataSet.cpp", "DistanceFunctions.cpp", "ConcurrentDataSet.cpp"],           # filename of our Cython source
        language="c++",              # this causes Cython to create C++ source
        include_dirs=['.'],          # usual stuff
        libraries=['pthread'],             # ditto
        ),]

setup(
    ext_modules = ext_modules,
    cmdclass = {'build_ext': build_ext}
)


