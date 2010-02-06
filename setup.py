from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext_modules = [
    Extension(
        "reccage",                 # name of extension
        [
            "src/pDataSet.pyx",
            "src/DataSet.cpp",
            "src/DistanceFunctions.cpp",
            "src/ConcurrentDataSet.cpp",
            "src/ConcurrentDataSetPool.cpp"
        ],
        # filename of our Cython source
        language="c++",              # this causes Cython to create C++ source
        include_dirs=['src'],          # usual stuff
        libraries=['pthread'],             # ditto
    ),
]

setup(
    name='Reccage',
    url='http://github.com/mikelikespie/reccage',
    author='Mike Lewis',
    author_email='mikelikespie@gmail.com',
    package_dir = {'reccage' : 'src'},
    packages=['reccage'],
    ext_modules = ext_modules,
    cmdclass = {'build_ext': build_ext}
)


