#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Expected program"
  exit 1
fi

GREEN="\033[32m"
RED="\033[31m"
CYAN="\033[36m"
YELLOW="\033[33m"
BOLD="\033[1m"
RESET="\033[0m"

PROGRAM=$1
script_dir="$(cd "$(dirname "$0")" && pwd)"
folder="${script_dir#$PWD/}"

printf "${GREEN}Start tests in${RESET} $folder\n"
total=0
failed=0

for testdir in "$script_dir"/test_*; do
  [ -d "$testdir" ] || continue
  ((total++))

  testname=$(basename "$testdir")

  rel_testdir="${testdir#$PWD/}"
  pcl="$rel_testdir/test.pcl"

  for stdin_file in "$testdir"/stdin_*.txt; do
    expected="${stdin_file/stdin_/expected_stdout_}"

    output="$(mktemp)"

    "$PROGRAM" "$pcl" < "$stdin_file" > "$output"
    status=$?

    if [ $status -ne 0 ]; then
      ((failed++))
      printf "${RED}Runtime error in\n${RESET}$pcl\n"
      echo "-----------------------------------------------------------"
      cat "$pcl"
      echo "-----------------------------------------------------------"
      rm "$output"
      continue
    fi

    if ! diff -q "$output" "$expected" >/dev/null; then
      ((failed++))
      printf "${RED}Test failed:${RESET} $testname\n"
      echo "--- Input:"
      cat "$stdin_file"
      echo "--- Expected:"
      cat "$expected"
      echo "--- Got:"
      cat "$output"
      echo "------------------------------------"
    fi

    rm "$output"
  done
done

if [ $failed -eq 0 ]; then
  printf "${GREEN}All $total tests passed${RESET}\n"
else
  printf "${RED}$failed/$total tests failed${RESET}\n"
fi
