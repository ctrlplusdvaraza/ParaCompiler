#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Expected program"
  exit 1
fi

script_dir="$(cd "$(dirname "$0")" && pwd)"

"$script_dir/erroneous_tests_for_lexer/test.sh" "$1"
"$script_dir/erroneous_tests_for_parser/test.sh" "$1"
"$script_dir/simple_tests/test.sh" "$1"
