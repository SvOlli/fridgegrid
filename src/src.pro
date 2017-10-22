
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

# make sure they exist
#mkpath( $${BINDIR} )
#mkpath( $${LIBDIR} )
#mkpath( $${DLLDIR} )
#mkpath( $${TESTDIR} )
#mkpath( $${TOOLSDIR} )

# store them for subdirs to use
cache( BINDIR,   set stash, BINDIR )
cache( LIBDIR,   set stash, LIBDIR )
cache( DLLDIR,   set stash, DLLDIR )
cache( TESTDIR,  set stash, TESTDIR )
cache( TOOLSDIR, set stash, TOOLSDIR )

# make sure that selfbuilt libs are available everywhere
LIBSPATH = -L$${LIBDIR}
cache( LIBS, add stash, LIBSPATH )

TEMPLATE = subdirs

SUBDIRS += fridgegrid

message( building for config $$CONFIG )
