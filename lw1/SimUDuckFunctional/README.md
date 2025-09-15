# SimUDuck - Функциональная версия

Это функциональная реализация приложения SimUDuck, где паттерн "Стратегия" реализован в функциональном стиле с использованием лямбда-функций и замыканий.

## Ключевые особенности функциональной реализации

### 1. Замыкания с состоянием
Вместо классов с полями для хранения состояния используются **mutable лямбда-функции** с замыканиями:

```cpp
// Пример: счётчик полётов в замыкании
inline FlyWithCallbackFunction CreateFlyWithWingsWithCallbackBehavior()
{
    return [flightCount = 0](const std::function<void()>& onSpecialFlight) mutable {
        ++flightCount;  // Состояние хранится в замыкании
        std::cout << "I'm flying with wings! Flight #" << flightCount << std::endl;
        
        if (flightCount % 2 == 0 && onSpecialFlight) {
            onSpecialFlight();
        }
    };
}
```

### 2. Функциональные типы вместо интерфейсов
Вместо абстрактных классов используются типы функций:

```cpp
using FlyFunction = std::function<void()>;
using FlyWithCallbackFunction = std::function<void(const std::function<void()>&)>;
using DanceFunction = std::function<void()>;
using QuackFunction = std::function<void()>;
```

### 3. Фабричные функции для создания поведений
Каждое поведение создаётся через фабричную функцию:

- `CreateFlyWithWingsWithCallbackBehavior()` - полёт с крыльями и счётчиком
- `CreateFlyNoWayWithCallbackBehavior()` - не летает  
- `CreateWaltzDanceBehavior()` - танцует вальс
- `CreateMinuetDanceBehavior()` - танцует менуэт
- `CreateNoDanceBehavior()` - не танцует
- `CreateQuackBehavior()` - крякает
- `CreateSqueakBehavior()` - пищит
- `CreateMuteQuackBehavior()` - молчит

### 4. Специальные поведения с состоянием

#### ModelDuck - поведение "до первого полёта"
```cpp
inline FlyWithCallbackFunction CreateFlyWithWingsBeforeFlightWithCallbackBehavior()
{
    return [flightCount = 0, firstFlight = true](const std::function<void()>& onSpecialFlight) mutable {
        if (firstFlight) {
            std::cout << "I'm a model duck and I can't fly" << std::endl;
            firstFlight = false;  // Состояние изменяется в замыкании
        } else {
            ++flightCount;
            std::cout << "I'm flying with wings! Flight #" << flightCount << std::endl;
            // ...
        }
    };
}
```

## Реализованный функционал

### ✅ Счётчик полётов через замыкания
- Каждая утка имеет свой независимый счётчик полётов
- Счётчик хранится в mutable лямбда-функции
- Состояние сохраняется между вызовами благодаря замыканию

### ✅ Танцы уток
- MallardDuck - танцует вальс
- RedheadDuck - танцует менуэт  
- RubberDuck и DecoyDuck - не танцуют
- ModelDuck - не танцует

### ✅ Кряканье после каждого второго полёта
- Утки крякают после каждого чётного полёта
- Реализовано через коллбек-функцию в поведении полёта

### ✅ Смена поведения во время выполнения
- Метод `SetFlyBehavior()` позволяет изменить поведение полёта
- ModelDuck демонстрирует это: сначала не может летать, потом получает способность

## Компиляция и запуск

```bash
mkdir cmake-build-debug
cd cmake-build-debug  
cmake ..
make

# Запуск основной программы
./SimUDuckFunctional

# Запуск тестов
./FunctionalDuckBehaviorTests
```

## Структура проекта после очистки

Функциональная реализация значительно упростила структуру проекта:

```
SimUDuckFunctional/
├── lib/Duck/
│   ├── Fly/FlyBehaviorFunctions.h       # Только функциональные поведения полёта
│   ├── Dance/DanceBehaviorFunctions.h   # Только функциональные поведения танцев  
│   ├── Quack/QuackBehaviorFunctions.h   # Только функциональные поведения кряканья
│   ├── Duck.h                           # Обновлённый базовый класс
│   └── [5 конкретных классов уток]      # MallardDuck, RedheadDuck, etc.
├── tests/FunctionalDuckBehaviorTests.cpp # Только функциональные тесты
└── main.cpp                             # Демонстрация функциональности
```

**Удалено из проекта:**
- ❌ 4 интерфейса (IFlyBehavior, IDanceBehavior, IQuackBehavior)
- ❌ 11 классов поведений (FlyWithWings, WaltzDanceBehavior, QuackBehavior, etc.)
- ❌ 4 mock-класса для тестов
- ❌ 1 старый файл тестов

**Итого:** Удалено **20 файлов**, оставлено только **13 файлов**!

## Преимущества функциональной реализации

1. **Компактность**: **На 61% меньше файлов** (13 вместо 33)
2. **Замыкания**: Элегантное решение для хранения состояния  
3. **Гибкость**: Легко создавать новые поведения "на лету"
4. **Современный C++**: Использует возможности C++23
5. **Тестируемость**: Поведения легко тестировать независимо
6. **Простота**: Нет необходимости в сложных иерархиях классов

## Сравнение с объектно-ориентированной версией

| Аспект | ООП версия | Функциональная версия |
|--------|------------|----------------------|
| Интерфейсы | Абстрактные классы | std::function типы |
| Состояние | Поля классов | Замыкания с mutable |
| Создание | new/make_unique | Фабричные функции |
| Файлов | ~15 заголовочных | ~3 заголовочных |
| Стиль | Классический ООП | Современный функциональный |
