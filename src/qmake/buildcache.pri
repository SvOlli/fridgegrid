
# only allow out of tree builds (like qt creator)
equals( _PRO_FILE_PWD_, $${OUT_PWD} ) {
   error( "in-tree builds are prohibited." );
}

# define common directories
BINDIR   = $${OUT_PWD}/bin
LIBDIR   = $${OUT_PWD}/tmp/lib
DLLDIR   = $${OUT_PWD}/dll
TESTDIR  = $${OUT_PWD}/test
TOOLSDIR = $${OUT_PWD}/tools
QMAKEDIR = $${_PRO_FILE_PWD_}/qmake

# store them for subdirs to use
cache( BINDIR,   set stash, BINDIR )
cache( LIBDIR,   set stash, LIBDIR )
cache( DLLDIR,   set stash, DLLDIR )
cache( TESTDIR,  set stash, TESTDIR )
cache( TOOLSDIR, set stash, TOOLSDIR )
cache( QMAKEDIR, set stash, QMAKEDIR )

# make sure that selfbuilt libs are available everywhere
LIBSPATH = -L$${LIBDIR}
cache( LIBS, add stash, LIBSPATH )
