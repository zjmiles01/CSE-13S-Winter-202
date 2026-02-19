#!/bin/bash
./calc > got.txt
ec=$?
echo "NOT ENOUGH INPUT" > expected.txt

if [ $ec -eq 0 ]; then
  echo "Expected non-zero exit code for not enough input (0 args), got 0"
  exit 1
fi

diff -u expected.txt got.txt > /dev/null
if [ $? -ne 0 ]; then
  echo "Output mismatch for NOT ENOUGH INPUT (0 args)"
  diff -u expected.txt got.txt
  exit 1
fi

exit 0
