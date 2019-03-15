#!/bin/sh

set -e

cd "$(dirname "${0}")"

if [ ! -d '.git' ]; then
   echo "this script works only on a git repository."
   exit 1
fi

version="$(git describe --tags | cut -f1-2 -d- | tr - .)"
head -1 debian/changelog | grep -q "(${version})" ||
   dch -v "${version}"
exec dpkg-buildpackage -S -d

