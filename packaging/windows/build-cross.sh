#!/bin/sh

MINGW="/opt/mxe"
PLATFORM="i686-w64-mingw32.static"

BUILDDIR="../../build-release"
PACKAGINGDIR="${BUILDDIR}/packaging"

if [ ! -x "${MINGW}/usr/bin/${PLATFORM}-qmake-qt5" ]; then
   cat <<EOF
For building, an mxe is needed with qt5 enabled, but I could not find
${MINGW}/usr/bin/${PLATFORM}-qmake-qt5

Either you don't have mxe at all, this script is not configured to use it.
Take a look at the top of
$(readlink -f "${0}")
EOF
exit 1
fi

set -ex

export PATH=${PATH}:${MINGW}/usr/bin

cd "$(dirname "${0}")"

# obtain version from debian changelog
version="$(head -1 ../debian/changelog | sed 's/.*(\([^)]*\)).*/\1/')"

MAKEFLAGS=-j4 make -C ../.. release QMAKE=${PLATFORM}-qmake-qt5

mkdir -p ${PACKAGINGDIR}

for i in ${BUILDDIR}/bin/* ${BUILDDIR}/lib/*.dll; do
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
outfile="$(grep OutFile fridgegrid-cross.nsi|cut -f2 -d\"|tr '\\' '/')"
relfile="$(echo ${outfile}|sed "s/-installer/-${version}-installer/")"

mv -v "${outfile}" "${relfile}"

