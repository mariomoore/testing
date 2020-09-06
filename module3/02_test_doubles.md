<!-- .slide: data-background="#111111" -->

# Atrapy

## (ang. test doubles)

___

## Rodzaje atrap

* dummy
* fake
* stub
* spy
* mock

___

## Dummy

* <!-- .element: class="fragment fade-in" --> najprostsza atrapa
* <!-- .element: class="fragment fade-in" --> nie robi nic, puste funkcje
* <!-- .element: class="fragment fade-in" --> ma za zadanie tylko spełnić wymagania sygnatur funkcji i może nie być w ogóle używany

### Oryginalne funkcje
<!-- .element: class="fragment fade-in" -->

```cpp
double Car::accelerate(int) { /* complicated implementation */ }
void sayHello(std::string name) { std::cout << "Hello " << name << '\n'; }
```
<!-- .element: class="fragment fade-in" -->

### Implementacja dummy
<!-- .element: class="fragment fade-in" -->

```cpp
double DummyCar::accelerate(int) { return 0.0; }
void dummyHello(std::string name) {}
```
<!-- .element: class="fragment fade-in" -->

___
<!-- .slide: style="font-size: 0.9em" -->

## Stub

* <!-- .element: class="fragment fade-in" --> trochę bardziej zaawansowana atrapa
* <!-- .element: class="fragment fade-in" --> może mieć logikę lub minimalną implementację
* <!-- .element: class="fragment fade-in" --> zwraca zdefiniowane przez nas wartości

### Oryginalne funkcje
<!-- .element: class="fragment fade-in" -->

```cpp
double Car::accelerate(int) { /* complicated implementation */ }
void sayHello(std::string name) { std::cout << "Hello " << name << '\n'; }
```
<!-- .element: class="fragment fade-in" -->

### Implementacja stuba
<!-- .element: class="fragment fade-in" -->

```cpp
double StubCar::accelerate(int value) {
    return value < 0 ? -10.0 : 10.0;
}
void stubHello(std::string name) {
    if (name == "anonymous") {
        throw std::logic_error("anonymous not allowed");
    }
}
```
<!-- .element: class="fragment fade-in" -->

___

## Fake

* <!-- .element: class="fragment fade-in" --> modeluje bardziej złożone interakcje niż stub
* <!-- .element: class="fragment fade-in" --> w praktyce w C++ nawet się go nie wyróżnia jako oddzielny twór
* <!-- .element: class="fragment fade-in" --> często fake == stub

Fake objects actually have working implementations, but usually take some shortcut which makes them not suitable for production (an InMemoryTestDatabase is a good example).
<!-- .element: class="fragment fade-in" -->

___

## Mock

* <!-- .element: class="fragment fade-in" --> weryfikuje zachowanie testowanego obiektu
* <!-- .element: class="fragment fade-in" --> sprawdza czy wywołał on na mocku oczekiwane funkcje w oczekiwany sposób

### Oryginalna funkcja
<!-- .element: class="fragment fade-in" -->

```cpp
double Car::accelerate(int) { /* complicated implementation */ }
```
<!-- .element: class="fragment fade-in" -->

### Implementacja mocka
<!-- .element: class="fragment fade-in" -->

```cpp
class MockCar : public Car {
    MOCK_METHOD(double, accelerate, (int), (override));
};
```
<!-- .element: class="fragment fade-in" -->

Mocks are pre-programmed with expectations which form a specification of the calls they are expected to receive. They can throw an exception if they receive a call they don't expect and are checked during verification to ensure they got all the calls they were expecting.
<!-- .element: class="fragment fade-in" -->

___

### Mockowanie funkcji spoza klasy

* <!-- .element: class="fragment fade-in" --> Zazwyczaj używa się stubów lub dummy
* <!-- .element: class="fragment fade-in" --> Można użyć GMock, ale komplikuje to kod

Zobacz [GoogleMock CookBook](https://github.com/google/googletest/blob/master/googlemock/docs/cook_book.md#mocking-free-functions)
<!-- .element: class="fragment fade-in" -->

___

## Spy

* <!-- .element: class="fragment fade-in" --> mock, który dodatkowo liczy ilość wywołań funkcji
* <!-- .element: class="fragment fade-in" --> w GMock spy == mock

Spies are stubs that also record some information based on how they were called. One form of this might be an email service that records how many messages it was sent.
<!-- .element: class="fragment fade-in" -->

___

## W skrócie

* <!-- .element: class="fragment fade-in" --> Dummy
  * Jego celem jest “zaspokojenie sygnatury”.
  * Nie są wykonywane na nim żadne interakcje.
* <!-- .element: class="fragment fade-in" --> Stub
  * Minimalna implementacja do interakcji między obiektami.
  * Metody void są puste.
  * Wartości zwracane przez składowe są domyślne dla danego typu lub zdefiniowane (“hard-coded”).
* <!-- .element: class="fragment fade-in" --> Fake
  * Zawiera implementację logiki, która imituje kod produkcyjny, ale w możliwie najprostszy sposób.
* <!-- .element: class="fragment fade-in" --> Mock
  * Weryfikuje zachowanie poprzez rejestrowanie czy dana interakcja została wykonana.
* <!-- .element: class="fragment fade-in" --> Spy
  * Weryfikuje zachowanie poprzez rejestrowanie ilości wykonanych interakcji.

Źródło: [Kurs TDD cz. 19](https://dariuszwozniak.net/posts/kurs-tdd-19-mock-stub-fake-spy-dummy)
<!-- .element: class="fragment fade-in" -->

___

## W praktyce

W C++ spotkamy się z takim uproszczeniem:
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> dummy ~= stub
  * atrapa bez logiki
* <!-- .element: class="fragment fade-in" --> stub == fake
  * atrapa z logiką
* <!-- .element: class="fragment fade-in" --> mock == spy
  * atrapa z rejestrowaniem akcji

Obiekty typu dummy i stub implementujemy sami. Są one tak proste, że nie potrzeba do nich żadnych frameworków.
<!-- .element: class="fragment fade-in" -->

Do obiektów typu mock najczęściej wykorzystuje się bibliotekę GMock.
<!-- .element: class="fragment fade-in" -->

[Zobacz przykłady atrap w repo Pizzas](https://github.com/coders-school/pizzas/blob/master/test/mocks/PizzaMock.hpp)
<!-- .element: class="fragment fade-in" -->
