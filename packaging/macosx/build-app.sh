#!/bin/sh

set -e
cd "$(dirname $0)"

QTDIR=/usr/local/Trolltech/Qt-4.7.0
export PATH=${PATH}:${QTDIR}/bin

PACKAGINGDIR=../../build/package
BUILDAPP=../../build-release/bin/fridgegrid.app
DEPLOYAPP=${PACKAGINGDIR}/fridgegrid.app
PLUGINDIR=Contents/PlugIns

if [ -d "${DEPLOYAPP}" ]; then
  rm -rf "${DEPLOYAPP}"
fi

make -C ../.. release

mkdir -p "$(dirname ${DEPLOYAPP})"
cp -a "${BUILDAPP}" "${DEPLOYAPP}"

mkdir -p "${DEPLOYAPP}/${PLUGINDIR}"
for plugin in ../../build-release/lib/*.dylib; do
  if [ -f ${plugin} ]; then
    echo "${plugin}"
    cp "${plugin}" "${DEPLOYAPP}/${PLUGINDIR}"
    for lib in $(otool -L ${plugin}|grep Qt|sed 's/(.*)//g'|tr -d ' \t'); do
      echo "  ${lib}"
      install_name_tool -change ${lib} \
        $(echo ${lib}|sed 's,.*/lib/,@executable_path/../Frameworks/,') \
        ${DEPLOYAPP}/${PLUGINDIR}/${plugin##*/}
    done
  fi
done

(cd "$(dirname ${DEPLOYAPP})";macdeployqt "$(basename ${DEPLOYAPP})" -verbose=2)
pwd
rm -f ../../../fridgegrid.dmg
hdiutil create ../../../fridgegrid.dmg -volname "FridgeGrid" -fs HFS+ -srcfolder "${DEPLOYAPP}"

