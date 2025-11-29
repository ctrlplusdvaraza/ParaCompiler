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

program=$1
script_dir="$(cd "$(dirname "$0")" && pwd)"
folder="${script_dir#$PWD/}"

printf "${GREEN}Start tests in${RESET} $folder\n"
n_all=0
n_falled=0
for file in "$folder"/*.pcl; do
  if [ -f "$file" ]; then

    output="$("$program" "$file" 2>&1)"
    status=$?

    ((n_all++))

    if [ $status -eq 0 ] || ! grep -q "Error during parsing" <<< "$output"; then
      ((n_falled++))
      printf "${RED}Test should not have been compiled${RESET}\n$file\n"
      echo "-----------------------------------------------------------"
      cat "$file"
      echo "-----------------------------------------------------------"
    fi
  fi
done

if [ $n_falled -eq 0 ]; then
    printf "${GREEN}All $n_all tests passed${RESET}\n"
else 
    printf "${RED}$n_falled/$n_all tests falled${RESET}\n"
fi
