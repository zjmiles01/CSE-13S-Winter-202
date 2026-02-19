#!/bin/bash
./calc 007 4 > got.txt
ec=$?
echo "BAD INPUT" > expected.txt

# Spec: "A tiny integer cannot start with 0 unless it is '0'" -> treat as not an integer input
if [ $ec -eq 0 ]; then
  echo "Expected non-zero exit code for leading zero input (007), got 0"
  exit 1
fi

diff -u expected.txt got.txt > /dev/null
if [ $? -ne 0 ]; then
  echo "Output mismatch for leading zero input (007)"
  diff -u expected.txt got.txt
  exit 1
fi

exit 0
