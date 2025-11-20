#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Usage: $0 <compiler_or_interpreter>"
  exit 1
fi

PROGRAM="$1"

GREEN="\033[32m"
RED="\033[31m"
CYAN="\033[36m"
YELLOW="\033[33m"
BOLD="\033[1m"
RESET="\033[0m"

script_dir="$(cd "$(dirname "$0")" && pwd)"
cd "$script_dir" || exit 1

total=0
failed=0

echo -e "${GREEN}Starting tests in ${RESET}$script_dir"

for testdir in test_*; do
    [ -d "$testdir" ] || continue
    ((total++))

    echo -e "${CYAN}Running $testdir ...${RESET}"

    pcl_file="$testdir/test.pcl"
    if [ ! -f "$pcl_file" ]; then
        echo -e "${RED}No test.pcl in $testdir${RESET}"
        ((failed++))
        continue
    fi

    # Компиляция / проверка
    "$PROGRAM" "$pcl_file" >/dev/null 2>&1
    status=$?

    if [ $status -ne 0 ]; then
        echo -e "${RED}Compilation failed in $testdir${RESET}"
        ((failed++))
        continue
    fi

    # Запуск тестов stdin_*.txt
    ok=1
    for stdin_file in "$testdir"/stdin_*.txt; do
        [ -f "$stdin_file" ] || continue

        num=$(echo "$stdin_file" | sed -E 's/.*stdin_([0-9]+)\.txt/\1/')
        expected_file="$testdir/expected_stdout_${num}.txt"

        if [ ! -f "$expected_file" ]; then
            echo -e "${YELLOW}Warning: no expected_stdout_${num}.txt for $stdin_file${RESET}"
            continue
        fi

        output="$(mktemp)"

        # Запуск программы
        "$PROGRAM" "$pcl_file" < "$stdin_file" > "$output" 2>/dev/null
        run_status=$?

        if ! diff -q "$output" "$expected_file" >/dev/null 2>&1 || [ $run_status -ne 0 ]; then
            echo -e "${RED}❌ Test failed: $testdir (stdin_${num})${RESET}"
            echo "Input:"
            cat "$stdin_file"

            echo "Expected:"
            cat "$expected_file"

            echo "Got:"
            cat "$output"

            echo "---------------------------------------------"
            ok=0
        fi

        rm "$output"
    done

    if [ $ok -eq 1 ]; then
        echo -e "${GREEN}✔ $testdir passed${RESET}"
    else
        ((failed++))
    fi

done

if [ $failed -eq 0 ]; then
    echo -e "${GREEN}All $total tests passed.${RESET}"
else
    echo -e "${RED}$failed/$total tests failed.${RESET}"
fi

