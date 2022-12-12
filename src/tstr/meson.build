project ('tstr', 'c')

gobjdep = dependency('gobject-2.0')

test1files=files('test1.c', 'tstr.c', '../tnumber/tnumber.c', '../tnumber/tint.c', '../tnumber/tdouble.c')
executable('test1', test1files, dependencies: gobjdep, install: false)

test2files=files('test2.c', 'tstr.c', 'tnumstr.c', '../tnumber/tnumber.c', '../tnumber/tint.c', '../tnumber/tdouble.c')
executable('test2', test2files, dependencies: gobjdep, install: false)

sourcefiles=files('main.c', 'tstr.c', 'tnumstr.c', '../tnumber/tnumber.c', '../tnumber/tint.c', '../tnumber/tdouble.c')
executable('tnumstr', sourcefiles, dependencies: gobjdep, install: false)
