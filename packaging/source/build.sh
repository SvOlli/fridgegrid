#!/bin/sh

set -e

cd "$(dirname $0)"
cd ../../..

find fridgegrid/LICENSE.GPL fridgegrid/Makefile fridgegrid/packaging fridgegrid/src -type f |
   grep -v -e 'src/.*Makefile' -e '\.pro\.user$' -e '\.gitignore' |
   xargs tar zcvf fridgegrid.tar.gz
advdef -z4 fridgegrid.tar.gz || true
