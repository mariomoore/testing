<!-- .slide: data-background="#111111" -->

# Wstrzykiwanie zależności

## Dependency injection (DI)

___
<!-- .slide: style="font-size: 0.9em" -->

## Problem #1 - połączenia sieciowe

Jak przetestować funkcję, która wysyła dane przez sieć?

```cpp
size_t send(int socket, const void *buffer, size_t length, int flags);
```

* <!-- .element: class="fragment fade-in" --> Co jeśli gniazdko sieciowe (socket) nie będzie otwarte?
* <!-- .element: class="fragment fade-in" --> Co jeśli flagi są źle ustawione?
* <!-- .element: class="fragment fade-in" --> Co jeśli połączenie nie zostało nawiązane?
* <!-- .element: class="fragment fade-in" --> Co jeśli połączenie zostanie zerwane przed wysłaniem danych?
* <!-- .element: class="fragment fade-in" --> Co jeśli połączenie zostanie zerwane w trakcie wysyłania danych?
* <!-- .element: class="fragment fade-in" --> Co jeśli nasze bufory sieciowe są zapchane i nie dostaniemy informacji o liczbie wysłanych bajtów?
* <!-- .element: class="fragment fade-in" --> Co jeśli opóźnienia na łączu (lagi) są bardzo duże?
* <!-- .element: class="fragment fade-in" --> Co jeśli testy wykonujemy współbieżnie i spróbujemy nawiązać na raz 1000 połączeń?

Część odpowiedzi znajdziesz w podręczniku funkcji send - `man 2 send`.
<!-- .element: class="fragment fade-in" -->

Programowanie sieciowe i rozproszone wymaga przetestowania wielu scenariuszy nietypowych z punktu widzenia zwykłego programowania.
<!-- .element: class="fragment fade-in" -->

___

## Problem #2 - baza danych

Jak przetestować funkcję, która wysyła zapytania do bazy danych?

```cpp
Response sendQuery(Query q);
```

* <!-- .element: class="fragment fade-in" --> Co się stanie, jeśli serwer bazy danych nie będzie aktywny?
  * <!-- .element: class="fragment fade-in" --> Nie będzie połączenia z serwerem bazy danych. Test nie przejdzie. Trzeba pamiętać, aby serwer był zawsze aktywny przed uruchomieniem testów.
* <!-- .element: class="fragment fade-in" --> Co się stanie, jeśli serwer bazy danych jest na innej maszynie?
  * <!-- .element: class="fragment fade-in" --> Możemy nie mieć połączenia z serwerem bazy danych. Test nie przejdzie. Trzeba pamiętać, aby mieć zawsze łączność z serwerem.
* <!-- .element: class="fragment fade-in" --> Co się stanie, jeśli serwer bazy danych jest obciążony?
  * <!-- .element: class="fragment fade-in" --> Odpowiedź zostanie zwrócona po długim czasie. Testy będą wykonywać się bardzo długo lub nie będą przechodzić z powodu zbyt długiego czasu oczekiwania.
* <!-- .element: class="fragment fade-in" --> Co się stanie, jeśli baza przechowuje terabajty danych?
  * <!-- .element: class="fragment fade-in" --> Wyszukiwanie danych może trwać zbyt długo. Testy mogą nie przechodzić.

___

## Problem 3 - funkcja z timeoutem

Jak przetestować funkcję, która czeka na wykonanie zdalnej procedury przez określony czas, po czym zgłasza błąd?

```cpp
template <typename T>
Response executeRemotely(IpAddr address, T function, Timeout timeout);
```

* <!-- .element: class="fragment fade-in" --> Czy test jednostkowy wykonujący się 2 minuty jest w porządku?
  * <!-- .element: class="fragment fade-in" --> Nie jest. UT-ki powinny dawać programiście praktycznie natychmiastową odpowiedź. Inne rodzaje testów (systemowe, integracyjne) mogą trwać odpowiednio dłużej.
* <!-- .element: class="fragment fade-in" --> Co się stanie jeśli timeout ustawimy na 0?
  * <!-- .element: class="fragment fade-in" --> Funkcja natychmiastowo się zakończy czy będzie czekać w nieskończoność i zawieszą nam się testy?
* <!-- .element: class="fragment fade-in" --> Czy zmiana timeoutu na 1ms na potrzeby testu będzie w porządku?
  * <!-- .element: class="fragment fade-in" --> A co jeśli maszyna będzie akurat obciążona i test potrwa 2ms? Czy test powinien zostać zgłoszony jako FAILED?

___
<!-- .slide: style="font-size: 0.85em" -->

## Odcinanie i zmiana zależności

Aby przeprowadzić skuteczne testy dla wspomnianych problemów należy odciąć wszelkie zależności od czasu, baz danych czy operacji sieciowych.
<!-- .element: class="fragment fade-in" -->

Tego typu operacje powinny być pozamykane w oddzielne klasy, które podczas testów zastąpimy czymś, co nie będzie sprawiać problemów. Przykładowo możemy użyć specjalnych wersji wyżej wspomnianych funkcji.
<!-- .element: class="fragment fade-in" -->

```cpp
size_t send(int socket, const void *buffer, size_t length, int flags) {
    return 10;
}

Response sendQuery(Request r) {
    return Response{r};
}

template <typename T>
Response executeRemotely(IpAddr address, T function, Timeout timeout) {
    if (timeout == 0) {
        throw RemoteExecutionTimeout{};
    } else {
        return Response{};
    }
}
```
<!-- .element: class="fragment fade-in" -->

___

## Wstrzykiwanie zależności

Technika, w której zamiast faktycznych obiektów wstawiamy do testów nasze własne implementacje w celu pozbycia się problemów lub uproszczenia testów nazywa się **wstrzykiwaniem zależności (ang. dependency injection)**.
<!-- .element: class="fragment fade-in" -->

Istnieje kilka różnych jej typów, ale najpopularniejszą jest wykorzystanie dynamicznego polimorfizmu w celu podmiany zależności. Posługując się interfejsami można podstawiać dowolne inne obiekty, które implementują dany interfejs (w C++ dziedziczą po tym interfejsie).
<!-- .element: class="fragment fade-in" -->

___

## Testujemy klasę Player

```cpp
class Player {
    std::vector<std::shared_ptr<Weapon>> weapons_;
    Weapon& currentWeapon_;
    // ...
public:
    Player(const std::shared_ptr<Weapon> & defaultWeapon)
        : weapons_.emplace_back(defaultWeapon)
        , currentWeapon_(*defaultWeapon)
    {}
    void shoot() { currentWeapon_.shoot(); }
    void reload() { currentWeapon_.reload(); }
    // ...
};

class Weapon {
public:
    virtual void shoot() = 0;
    virtual void reload() = 0;
    virtual ~Weapon() = default;
};

class Machinegun : public Weapon {
    static size_t capacity_ = 40;
    size_t bullets_ = 40;
public:
    void shoot() override { --bullets_; }
    void reload() override { bullets_ = 40; };
};

```

___

### Użycie klasy Player w kodzie

```cpp
auto ak47 = std::make_shared<Machinegun>();
Player grzesiek{ak47};
grzesiek.shoot();
// shoot 40 times
grzesiek.reload();
// no possibility to check if mp5 has shoot, no such interface
```

___
<!-- .slide: style="font-size: 0.8em" -->

### Użycie w testach

Testujemy klasę `Player`, wycinamy zależność - `Machinegun`. Klasa `Machinegun` nie ma możliwości sprawdzenia czy strzeliliśmy, bo jej interfejs nie udostępnia sprawdzania pojemności magazynka.
<!-- .element: class="fragment fade-in" -->

```cpp
class TestWeapon : public Weapon {
public:
    void shoot() override { hasShoot = true; }
    void reload() override { hasShoot = false; }
    bool hasShoot = false;
};
```
<!-- .element: class="fragment fade-in" -->

Klasa `TestWeapon` posiada publiczne pole mówiące o tym, czy broń wystrzeliła. Dziedziczy ona po interfejsie `Weapon`, więc możemy jej użyć zamiast prawdziwej broni w testach.
<!-- .element: class="fragment fade-in" -->

```cpp
TEST(PlayerTest, shootingAndReloadingWeapon) {
    auto testWeapon = std::make_shared<TestWeapon>;
    Player testPayer{testWeapon};
    EXPECT_FALSE(testWeapon.hasShoot);
    testPlayer.shoot();
    EXPECT_TRUE(testWeapon.hasShoot);
    testPlayer.reload();
    EXPECT_FALSE(testWeapon.hasShoot);
}
```
<!-- .element: class="fragment fade-in" -->

Jeśli klasa `Player` poprawnie używa klasy `Weapon` to test powinien przejść.
<!-- .element: class="fragment fade-in" -->

___

## Wstrzykiwanie zależności z użyciem interfejsów

Wstrzykiwanie zależności pozwala uprościć kod testów. Nie musimy np. wywołać metody `shoot()` 40 razy aby rozładować magazynek. Gdyby klasa `Machinegun` potrzebowała w konstruktorze obiektu klasy `Magazine`, a klasa `Magazine` 40 obiektów klasy `Bullet`, to w testach również trzeba byłoby je utworzyć.
<!-- .element: class="fragment fade-in" -->

Pisanie własnej klasy `TestWeapon` to również pewien narzut, ale frameworki do wstrzykiwania zależności pozwalają nam to zrobić nawet w jednej linijce kodu.
<!-- .element: class="fragment fade-in" -->

___

## Co wycinać?

W **testach jednostkowych** wszystko poza testowaną klasą.
<!-- .element: class="fragment fade-in" -->

W **testach modułowych** wszystko poza testowanym modułem (kilka, kilkanaście klas realizujących jedną większą funkcjonalność).
<!-- .element: class="fragment fade-in" -->

W **testach systemowych** zewnętrzne systemy - bazy danych, sieć, zależności czasowe. Testujemy działanie całego systemu.
<!-- .element: class="fragment fade-in" -->

W **testach integracyjnych** nic. Testujemy współdziałanie systemu z prawdziwymi zależnościami.
<!-- .element: class="fragment fade-in" -->

___

## Inne techniki

* <!-- .element: class="fragment fade-in" --> split backend - wstrzykiwanie zależności przez linkowanie z inną implementacją. Należy w tym celu odpowiednio skonfigurować system budowania (Makefile, CMakeLists.txt), aby podczas budowania testów kompilował i linkował z podmienionymi implementacjami.
* <!-- .element: class="fragment fade-in" --> split header - wstrzykiwanie zależności przez includowanie innych plików nagłówkowych do testów.
* <!-- .element: class="fragment fade-in" --> wstrzykiwanie przez settery - mało eleganckie, w szczególności gdy musimy napisać setter specjalnie do testów.
