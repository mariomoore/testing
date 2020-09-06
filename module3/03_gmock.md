<!-- .slide: data-background="#111111" -->

# GMock

___

## GMock - dokumentacja

* [GMock for Dummies](https://github.com/google/googletest/blob/master/googlemock/docs/for_dummies.md)
* [GMock cheatsheet](https://github.com/google/googletest/blob/master/googlemock/docs/cheat_sheet.md)
* [GMock cookbook](https://github.com/google/googletest/blob/master/googlemock/docs/cook_book.md)

___

## Tworzenie mocków

1. Pisanie z palca
2. [Użycie skryptu generującego](https://github.com/google/googletest/tree/master/googlemock/scripts/generator)

___

```cpp
class Foo {
  ...
  virtual ~Foo();
  virtual int GetSize() const = 0;
  virtual string Describe(const char* name) = 0;
  virtual string Describe(int type) = 0;
  virtual bool Process(Bar elem, int count) = 0;
};
```

```cpp
#include "gmock/gmock.h"

class MockFoo : public Foo {
  ...
  MOCK_METHOD(int, GetSize, (), (const, override));
  MOCK_METHOD(string, Describe, (const char* name), (override));
  MOCK_METHOD(string, Describe, (int type), (override));
  MOCK_METHOD(bool, Process, (Bar elem, int count), (override));
};
```
<!-- .element: class="fragment fade-in" -->

___

## Ustawianie oczekiwań

```cpp
MockFoo foo;
// ...
EXPECT_CALL(foo, Describe(5))
      .Times(3)
      .WillRepeatedly(Return("Category 5"));
```
<!-- .element: class="fragment fade-in" -->

Oczekujemy, że na `foo` zostanie 3-krotnie zawołana funkcja `Describe` z parametrem `5` i ustawiamy, że każdorazowo zwróci ona `std::string` o treści "Category 5".
<!-- .element: class="fragment fade-in" -->

```cpp
EXPECT_CALL(foo, Process(_, 10))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
```
<!-- .element: class="fragment fade-in" -->

Oczekujemy, że na `foo` zostanie zawołana funkcja `Process` z obojętnie jakim pierwszym parametrem i 10 jako drugi parametr. Za pierwszym razem zwróci true, za drugim false.
<!-- .element: class="fragment fade-in" -->

[Więcej możliwości w GoogleMock CheatSheet](https://github.com/google/googletest/blob/master/googlemock/docs/cheat_sheet.md#setting-expectations-expectcall)
<!-- .element: class="fragment fade-in" -->

___
<!-- .slide: style="font-size: 0.85em" -->

## Dekoratory mocków

### Nice Mock
<!-- .element: class="fragment fade-in" -->

```cpp
NiceMock<MockFoo> nice_foo;      // The type is a subclass of MockFoo.
```
<!-- .element: class="fragment fade-in" -->

Ignoruje dodatkowe funkcje, które zostały wywołane oprócz funkcji oczekiwanych.
<!-- .element: class="fragment fade-in" -->

### Naggy Mock (domyślny)
<!-- .element: class="fragment fade-in" -->

```cpp
MockFoo foo;
NaggyMock<MockFoo> naggy_foo;    // The type is a subclass of MockFoo.
```
<!-- .element: class="fragment fade-in" -->

Wyświetla ostrzeżenia o dodatkowych funkcjach, które zostały wywołane oprócz tych oczekiwanych.
<!-- .element: class="fragment fade-in" -->

### Strict Mock
<!-- .element: class="fragment fade-in" -->

```cpp
StrictMock<MockFoo> strict_foo;  // The type is a subclass of MockFoo.
```
<!-- .element: class="fragment fade-in" -->

Wywołania dodatkowych funkcji poza oczekiwanymi są traktowane jak błędy, przez które test nie przechodzi.
<!-- .element: class="fragment fade-in" -->

___
<!-- .slide: style="font-size: 0.95em" -->

## Zadanie

### [Repo Pizzas](https://github.com/coders-school/pizzas)

0. Popraw błędy w programie, aby testy przechodziły
1. Popraw klasę PizzaMock, aby była napisana z użyciem nowszej wersji GMocka.
2. Usuń zależność od czasu w testach za pomocą dummy lub stuba.

___
<!-- .slide: style="font-size: 0.85em" -->

### 1. Usuń zależność od czasu w testach

#### W którym miejscu uzależniamy się od czasu?
<!-- .element: class="fragment fade-in" -->

`Pizzeria.cpp:52`
`std::this_thread::sleep_for(pizza->getBakingTime());`
<!-- .element: class="fragment fade-in" -->

#### W jaki sposób można wyciąć tę zależność?
<!-- .element: class="fragment fade-in" -->

Należy utworzyć interfejs, po którym będą dziedziczyć 2 implementacje.
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Pierwsza z nich będzie faktycznie wywoływać obecną funkcję `std::this_thread::sleep_for()` i będzie używana w zwykłej binarce.
* <!-- .element: class="fragment fade-in" --> Druga z nich nie będzie nic robiła i będzie używana w testach.

#### W jaki sposób wstrzykiwać te zależności?
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> W konstruktorze klasy <code>Pizzeria</code> należy dodać dodatkowy argument - wskaźnik/referencję do interfejsu.
* <!-- .element: class="fragment fade-in" --> Klasa <code>Pizzeria</code> powinna posiadać nowe pole - również wskaźnik/referencję do interfejsu.
* <!-- .element: class="fragment fade-in" --> W miejscu dawnego wywołania funkcji <code>std::this_thread::sleep_for()</code> należy wywołać odpowiednią funkcję z interfejsu.
