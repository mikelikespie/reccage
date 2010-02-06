from distutils.core import setup
from distutils.extension import Extension


cython_files = ['src/pDataSet.pyx']
alternative_cpp_files = ['src/pDataSet.cpp']

regular_cpp_files = [
    "src/DataSet.cpp",
    "src/DistanceFunctions.cpp",
    "src/ConcurrentDataSet.cpp",
    "src/ConcurrentDataSetPool.cpp"
]

wrapper_files = cython_files

try:
    from Cython.Distutils import build_ext
except ImportError:
    from distutils.command.build_ext import build_ext
    wrapper_files = alternative_cpp_files


ext_modules = [
    Extension(
        "reccage",                 # name of extension
        regular_cpp_files + alternative_cpp_files,
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


