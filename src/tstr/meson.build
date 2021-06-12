project ('tstr', 'c')

gobjdep = dependency('gobject-2.0')

sourcefiles=files('main.c', 'tstr.c', 'tnumstr.c', '../tnumber/tnumber.c', '../tnumber/tint.c', '../tnumber/tdouble.c')

executable('tstr', sourcefiles, dependencies: gobjdep, install: false)

