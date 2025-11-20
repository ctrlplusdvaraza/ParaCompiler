#!/bin/bash

script_dir="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$(cd "$script_dir/../.." && pwd)"   # корень проекта
PROGRAM="$ROOT_DIR/start.sh"

if [ ! -f "$PROGRAM" ]; then
    echo "❌ start.sh не найден в корне проекта: $PROGRAM"
    exit 1
fi

GREEN="\033[32m"
RED="\033[31m"
CYAN="\033[36m"
RESET="\033[0m"

cd "$script_dir" || exit 1

total=0
failed=0

echo -e "${GREEN}Starting tests in:${RESET} $script_dir"

for testdir in test_*; do
    [ -d "$testdir" ] || continue
    ((total++))

    echo -e "${CYAN}Running $testdir ...${RESET}"

    pcl="$testdir/test.pcl"
    if [ ! -f "$pcl" ]; then
        echo -e "${RED}Missing test.pcl in $testdir${RESET}"
        ((failed++))
        continue
    fi

    ok=1

    for stdin_file in "$testdir"/stdin_*.txt; do
        [ -f "$stdin_file" ] || continue

        num=$(echo "$stdin_file" | sed -E 's/.*stdin_([0-9]+)\.txt/\1/')
        expected="$testdir/expected_stdout_${num}.txt"

        if [ ! -f "$expected" ]; then
            echo -e "${RED}Missing expected_stdout_${num}.txt${RESET}"
            ok=0
            continue
        fi

        output="$(mktemp)"

        "$PROGRAM" "$pcl" < "$stdin_file" > "$output"
        status=$?

        if [ $status -ne 0 ]; then
            echo -e "${RED}❌ Runtime error in $testdir (test $num)${RESET}"
            ok=0
            rm "$output"
            continue
        fi

        if ! diff -q "$output" "$expected" >/dev/null; then
            echo -e "${RED}❌ $testdir test $num failed${RESET}"
            echo "--- Input:"
            cat "$stdin_file"
            echo "--- Expected:"
            cat "$expected"
            echo "--- Got:"
            cat "$output"
            echo "------------------------------------"
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
    echo -e "${RED}$failed / $total tests failed.${RESET}"
fi

