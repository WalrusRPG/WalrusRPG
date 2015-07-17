#!/bin/bash

COMMIT_NUMBER=$(git describe --always)
BRANCH_NAME=$(git rev-parse --abbrev-ref HEAD)
VERSION="$BRANCH_NAME-$COMMIT_NUMBER"

source=$(cat <<EOF
const char git_version[] = "$VERSION";
EOF
)

diff $1 <(echo "$source") > /dev/null 2>&1
if [[ $? -ne 0 ]]
  then
  echo "Updating version source"
  echo "$source" > $1
fi
