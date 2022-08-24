
dep_xxhash = dependency('libxxhash')
dep_png    = dependency('libpng')
dep_thread = dependency('threads')
dep_openmp = dependency('openmp',   required: false)
dep_common = [dep_xxhash, dep_thread, dep_png, dep_openmp]

stfiles = [
  'level1.c', 'spec1.c',
  'level2.c', 'spec2.c', 'raw_bit2.c',
  'level3.c', 'spec3.c', 'raw_bit3.c',
  'dkl.c', 'dkc_gbc.c',
  'gba_misc.c', 'misc.c',
  'dkc_gba.c', 'dkc2_gba.c', 'dkc2_decomp.c',
  'jc.c', 'kos.c',
  'bitplane.c', 'png.c', 'rom.c',
]

lib_dkcomp = subproject('dkcomp').get_variable('libdkcomp')

lib_stripper = shared_library('stripper',
    stfiles,
    dependencies: [dep_common],
    link_with:    [lib_dkcomp]
)

