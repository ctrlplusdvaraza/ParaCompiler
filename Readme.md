# Компилятор для языка ParaCL

## Зависимости
- Flex (версия 2.6+)
- Bison (версия 3.8+)
- Protobuf (версия синтаксиса proto 3)
    - Abseil (версия 20250814.1-1)

## Компиляция
### Linux / MacOS
```sh
./build.sh [--debug|--release] [--build-dir <dir>]
```

### Windows
```
cmake -DCMAKE_BUILD_TYPE=[Release | Debug] -DSANITIZE=[TRUE | FALSE] .
cmake --build ./build
```

## Запуск
### Linux / MacOS
```sh
./run.sh <file_name> [--graphviz] [--build-dir <dir>]
```

### Windows
``` ./build/frontend/frontend.exe <file_name> ``` - создание ast дерева
``` ./build/graphviz/graphviz.exe <ast_file> ``` - создание svg представления ast дерева
``` ./build/simulator/simulator.exe <ast_file> ``` - запуск эмулятора языка
