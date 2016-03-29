#!/bin/bash

COMMIT_NUMBER=$(git describe --always 2> /dev/null) 
BRANCH_NAME=$(git rev-parse --abbrev-ref HEAD 2> /dev/null)
VERSION="$BRANCH_NAME-$COMMIT_NUMBER"

source=$(cat <<EOF
const char git_version[] = "$VERSION";
EOF
) || "WalrusRPG-undefined"

echo $source | diff $1 - > /dev/null 2>&1
if [[ $? -ne 0 ]]
  then
  echo "Updating version source"
  echo "$source" > $1
fi
