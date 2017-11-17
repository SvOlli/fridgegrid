#############################################################################
# src/buildconfig.pri
# hand hacked by SvOlli
# released as public domain
#############################################################################

contains( TEMPLATE, app ) {
  is_test = $$find( OUT_PWD, "/tests/" )
  count( is_test, 1 ) {
    DESTDIR    = $${TESTDIR}
  } else {
    DESTDIR    = $${BINDIR}
  }
} else {
  contains( CONFIG, plugin ) {
    DESTDIR    = $${DLLDIR}
  } else {
    DESTDIR    = $${LIBDIR}
  }
}

contains( QMAKE_CXX, g++ ) {
  QMAKE_CXXFLAGS_DEBUG += -pedantic -Wall -Wno-long-long
  QMAKE_CXXFLAGS_RELEASE += -ffunction-sections -fdata-sections
  !macx {
    QMAKE_LFLAGS_RELEASE += -Wl,--gc-sections -rdynamic
  }
}
