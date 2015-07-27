#!/bin/bash

header="#include <stdint.h>\n"$(cat $1 |grep "=" |sed -e "s/ =.*/;/" -e "s/^/extern /")

echo -e "$header" | diff - $2 > /dev/null 2>&1
if [[ $? -ne 0 ]]
  then
  echo "Making header $2"
  echo -e "$header" > $2
fi
