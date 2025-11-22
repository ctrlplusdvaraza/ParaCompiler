# Компилятор для языка ParaCL

## Зависимости
- Flex (версия 2.6 и выше)
- Bison (версия 3.8 и выше)
- Protobuf (версия синтаксиса proto 3)
    - Abseil (версия 20250814.1-1 и выше)

## Компиляция
### Linux / MacOS
```sh
./build.sh [--debug | --release] [--build-dir <dir>]
```

### Windows
```
cmake -DCMAKE_BUILD_TYPE=[Release | Debug] -DSANITIZE=[TRUE | FALSE] .
cmake --build ./build
```

## Запуск
### Linux / MacOS
```sh 
./run.sh <file_name> [--graphvizer] [--build-dir <dir>] 
```

### Windows
Создание AST дерева исходного кода
```
./build/frontend/frontend.exe <file_name>
```

Запуск эмулятора языка
```
./build/simulator/simulator.exe <ast_file>
```

Генерация SVG представления AST дерева
```
./build/graphvizer/graphvizer.exe <ast_file>
```

