<!-- .slide: data-background="#111111" -->

# Praca domowa

___
<!-- .slide: style="font-size: 0.9em" -->

## Post-work

### [Repo Pizzas](https://github.com/ziobron/Pizzas)

0. <!-- .element: class="fragment fade-in-then-semi-out" --> Popraw błędy w programie, aby testy przechodziły
1. <!-- .element: class="fragment fade-in-then-semi-out" --> Usuń zależność od czasu w testach za pomocą dummy lub stuba.
2. <!-- .element: class="fragment fade-in-then-semi-out" --> Popraw klasę PizzaMock, aby była napisana z użyciem nowszej wersji GMocka.
3. <!-- .element: class="fragment fade-in" --> (Praca domowa) - 15 XP. Napisz klasę TimeMock, którą zastąpisz dummy/stuba z punktu 1. Używaj go poprzez StrictMock. Dodaj właściwe <code>EXPECT_CALL</code> w kodzie testów.
4. <!-- .element: class="fragment fade-in" --> (Praca domowa) - 15 XP. Dopisz nowy przypadek testowy, w którym powielasz działanie funkcji main.cpp, ale zamawiasz 3 pizze: <code>StubPizza</code> oraz 2 różne <code>MockPizza</code> (różne nazwy, ceny i czas pieczenia; jedna jako StrictMock, druga jako NiceMock). Ustaw właściwe <code>EXPECT_CALL</code>.

Możecie pracować w parach 🙂
<!-- .element: class="fragment fade-in" -->

___

## Pre-work

* Znajdź i przeczytaj czym są:
  * RAII
  * wycieki pamięci i jak je wykrywać
  * wyjątki i jak ich używać w C++
