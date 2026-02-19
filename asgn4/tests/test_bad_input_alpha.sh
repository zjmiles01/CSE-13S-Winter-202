#!/bin/bash
./calc xyz 4 > got.txt
ec=$?
echo "BAD INPUT" > expected.txt

if [ $ec -eq 0 ]; then
  echo "Expected non-zero exit code for BAD INPUT, got 0"
  exit 1
fi

diff -u expected.txt got.txt > /dev/null
if [ $? -ne 0 ]; then
  echo "Output mismatch for BAD INPUT"
  diff -u expected.txt got.txt
  exit 1
fi

exit 0
