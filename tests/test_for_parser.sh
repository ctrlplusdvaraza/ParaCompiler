#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Expected program"
  exit 1
fi

program=$1
folder="./erroneous_tests_for_parser"

for file in "$folder"/*; do
  if [ -f "$file" ]; then
    echo "▶ Запуск '$program' с файлом '$file'..."
    "$program" "$file" > /dev/null 2>&1
    status=$?

    if [ $status -ne 0 ]; then
      echo "❌ Программа завершилась с ошибкой ($status) на файле: $file"
    else
      echo "✅ Программа завершилась успешно на файле: $file"
    fi
    echo "------------------------------------"
  fi
done

