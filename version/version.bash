#!/bin/bash

COMMIT_NUMBER=$(git describe --always)
BRANCH_NAME=$(git rev-parse --abbrev-ref HEAD)
VERSION="$BRANCH_NAME-$COMMIT_NUMBER"

DIRNAME=$(dirname "$0")

source=$(cat <<EOF
#include "version.h"

const char git_version[] = "$VERSION";
EOF
)

diff $DIRNAME/version.c <(echo "$source") > /dev/null 2>&1
if [[ $? -ne 0 ]]
  then
  echo "Updating version source"
  echo "$source" > $DIRNAME/version.c
fi
