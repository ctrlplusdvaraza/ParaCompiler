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

```sh 
./build/paracl [--graphviz[=output_file]] [--ast[=output_file]] [--no-simulation] <source_file>
```

Если выходной файл для опций --graphviz (-g) и --ast (-a) не указан, то используется файл по умолчанию.

### Запуск отдельных модулей компилятора (Linux):

Фронтенд (построение AST-дерева):
```sh
./build/frontend/frontend <file_name>
```

Симулятор (исполнение программы на ParaCL):
```sh
./build/simulator/simulator <ast_file>
```

Визуализатор AST (генерация SVG-графа):
```sh
./build/graphviz/graphviz <ast_file> <img_file>
```

 ## Тестирование (Linux)
```sh
./tests/run_tests.sh ./build/paracl
```
