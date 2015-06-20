#!/bin/bash

COMMIT_NUMBER=$(git describe --always)
BRANCH_NAME=$(git rev-parse --abbrev-ref HEAD)
VERSION="$BRANCH_NAME-$COMMIT_NUMBER"

source=$(cat <<EOF
#include "version.h"

const char git_version[] = "$VERSION";
EOF
)

include=$(cat <<EOF
#ifndef INCLUDE_VERSION_H
#define INCLUDE_VERSION_H
extern const char git_version[];
#endif
EOF
)

diff src/version.c <(echo "$source")
if [[ $? -ne 0 ]]
  then
  echo "Updating version source"
  echo "$source" > src/version.c
fi
diff include/version.h <(echo "$include")
if [[ $? -ne 0 ]]
  then
  echo "Updating version include"
  echo "$include" > include/version.h
fi
