#!/bin/bash
./calc 513 0 > got.txt
ec=$?
echo "TOO BIG" > expected.txt

if [ $ec -eq 0 ]; then
  echo "Expected non-zero exit code for TOO BIG (513), got 0"
  exit 1
fi

diff -u expected.txt got.txt > /dev/null
if [ $? -ne 0 ]; then
  echo "Output mismatch for TOO BIG (513)"
  diff -u expected.txt got.txt
  exit 1
fi

exit 0
