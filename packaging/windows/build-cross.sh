#!/bin/sh

MINGW=/opt/mxe
PLATFORM=i686-pc-mingw32

PACKAGINGDIR=../../build/packaging

set -e

export PATH=${PATH}:${MINGW}/usr/bin

cd "$(dirname $0)"
(cd ../..
#rm -rf build
#${PLATFORM}-qmake -r CONFIG+=debug_and_release
MAKEFLAGS=-j4 make release QMAKE=${PLATFORM}-qmake)

mkdir -p ${PACKAGINGDIR}

for i in ../../build/release/bin/* ../../build/release/lib/*.dll; do
  if [ -f "${i}" ]; then
    INSTALLFILES="${INSTALLFILES} ${i}"
  fi
done

QTLIBS="$(${PLATFORM}-objdump -x ${INSTALLFILES} | tr ' ' '\n' | grep ^Qt.*dll$ | sort -u | sed s,^,${MINGW}/usr/${PLATFORM}/bin/,)"

rm -f ${PACKAGINGDIR}/files-install.nsh ${PACKAGINGDIR}/files-delete.nsh
for i in ${INSTALLFILES} ${QTLIBS};do
  echo "  File \"$(echo ${i}|tr '/' '\\')\"" \
    >> ${PACKAGINGDIR}/files-install.nsh
  echo "  Delete \$INSTDIR\\${i##*/}" \
    >> ${PACKAGINGDIR}/files-delete.nsh
done

${PLATFORM}-makensis fridgegrid-cross.nsi

