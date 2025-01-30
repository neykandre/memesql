# memesql

**memesql** — это минималистичная библиотека на C++ для работы с in-memory базами данных, реализующая простую SQL-подобную систему управления данными.

## 🚀 Возможности

- Создание и управление таблицами
- Поддержка базовых SQL-операций (`CREATE TABLE`, `INSERT INTO`)
- Работа с различными типами данных (`int32`, `string`, `bytes`, `bool`)
- Использование уникальных и автоинкрементных ключей

## 🛠 Технологии

- **C++20**
- **CMake** для сборки

## 🔧 Установка и сборка

### 1. Клонирование репозитория

```sh
  git clone https://github.com/neykandre/memesql.git
  cd memesql
```

### 2. Сборка с CMake

```sh
  mkdir build && cd build
  cmake ..
  make
```

### 3. Запуск примера

```sh
  ./main
```

## 📜 Пример использования

Пример работы с `memesql`:

```cpp
#include "DataBase.hpp"
using namespace memesql;

int main() {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id : int32, {unique} login: string[32], password_hash: bytes[8], is_admin: bool = false)");
    db.execute("insert into users (login, password_hash, is_admin) values ('admin', '12345678', true)");
    return 0;
}
```

## 👨‍💻 Авторы

- **neykandre** (GitHub)

