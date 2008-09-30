from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext



ext_modules = [Extension(
        "DataSet",                 # name of extension
        ["pDataSet.pyx", "DataSet.cpp", "DistanceFunctions.cpp", "ConcurrentDataSet.cpp"],           # filename of our Cython source
        language="c++",              # this causes Cython to create C++ source
        include_dirs=['.'],          # usual stuff
        libraries=['pthread'],             # ditto
        extra_link_args=[],       # if needed
        cmdclass = {'build_ext': build_ext}),]

setup(
    cmdclass = {'build_ext': build_ext},
    ext_modules = ext_modules

)


