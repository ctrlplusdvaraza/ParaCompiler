# Компилятор для языка ParaCL

Компилятор и эмулятор языка программирования ParaCL с поддержкой AST визуализации.

## Зависимости
- Flex (версия 2.6 и выше) — генератор лексических анализаторов.
- Bison (версия 3.8 и выше) — генератор парсеров.
- Protobuf (syntax = "proto3") — сериализация данных.
  - Abseil (версия 20250814.1-1 и выше) — библиотека поддержки Protobuf.
- Graphviz Dot (версия 12.1.0 и выше) — генератор графических представлений.

## Компиляция (Linux)

### Вариант 1 - используя скрипт:
```sh
./build.sh [--debug | --release]
```

### Вариант 2 - вручную:

```sh
mkdir -p ./build
cd ./build

cmake ..
cmake --build .
```

## Запуск (Linux)

### Вариант 1 - используя скрипт:
```sh 
./run.sh <file_name> [--graphviz] 
```

### Вариант 2 - вручную:

Создание AST дерева исходного кода:
```sh
./build/frontend/frontend <file_name>
```

Запуск эмулятора языка:
```sh
./build/simulator/simulator <ast_file>
```

Генерация SVG представления AST дерева:
```sh
./build/graphvizer/graphvizer <ast_file>
```

Тестирование:
```sh
./tests/run_tests.sh ./run.sh
```
