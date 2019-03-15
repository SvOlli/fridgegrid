#!/bin/bash

set -e
cd "$(dirname $0)"

QTDIR="${HOME}/Qt5.11.1/5.11.1/clang_64"
if [ -x "${QTDIR}/bin/qmake" ]; then
  echo "QTDIR does not exist or does not contain Qt tools."
  exit 1
fi
export PATH="${PATH}:${QTDIR}/bin"

# obtain version from debian changelog
version="$(head -1 ../debian/changelog | sed 's/.*(\([^)]*\)).*/\1/')"

PACKAGINGDIR="../../build/package"
BUILDAPP="../../build-release/bin/fridgegrid.app"
DEPLOYAPP="${PACKAGINGDIR}/FridgeGrid.app"
PLUGINDIR="Contents/PlugIns"

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
rm -f ../../../FridgeGrid-${version}.dmg
hdiutil create ../../../FridgeGrid-${version}.dmg -volname "FridgeGrid ${version}" -fs HFS+ -srcfolder "${DEPLOYAPP}"

