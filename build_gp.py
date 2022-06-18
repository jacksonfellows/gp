import cffi

ffi = cffi.FFI()

with open('./gp.h') as f:
    ffi.cdef(f.read())

ffi.set_source(
    '_gp',
    '#include "gp.h"',
    libraries=['gp'],
    library_dirs=['.']
)

ffi.compile()
