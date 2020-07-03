#!/bin/sh

./piyojava Test 1> actual 2>check.log
if [ $? -ne 0 ]; then
  echo RUNTIME ERR!
  exit 1
fi
diff expected actual
if [ $? -ne 0 ]; then
  echo NG
  exit 1
fi
echo OK
