# Parking Gate Simulation System

## Opis projektu

Projekt symulacji parkingu z automatycznie sterowanym szlabanem realizowany z wykorzystaniem mikrokontrolera (np. NXP KL05Z lub STM32). System symuluje działanie wjazdu na parking poprzez wykrycie nadjeżdżającego pojazdu, otwarcie/zamknięcie szlabanu oraz sygnalizację świetlną.

## Funkcjonalności

- Wykrywanie pojazdu za pomocą czujnika ultradźwiękowego (np. HC-SR04)
- Automatyczne otwieranie i zamykanie szlabanu sterowanego serwomechanizmem
- Sygnalizacja świetlna LED (np. czerwone/żółte/zielone diody)
- Alarm dźwiękowy (opcjonalny) w przypadku nieprawidłowego działania
- Bezpieczne opóźnienia czasowe (np. po wykryciu pojazdu)
- Prosty stanowy algorytm obsługi całego procesu

## Użyte technologie i narzędzia

- **Mikrokontroler:** NXP KL05Z / STM32
- **Język programowania:** C
- **Środowisko:** MCUXpresso / STM32CubeIDE
- **Sprzęt:**
  - Czujnik HC-SR04
  - Serwomechanizm (np. SG90)
  - Diody LED
  - Buzzer (opcjonalnie)
- **Dodatkowe elementy:**
  - Zasilanie z USB
  - Przewody, płytka stykowa, rezystory

## Struktura projektu

```
/ParkingSimulation
│
├── src/                    # Pliki źródłowe (main.c, ultrasonic.c, gate.c)
├── inc/                    # Pliki nagłówkowe
├── Drivers/                # Biblioteki HAL / SDK
├── README.md               # Opis projektu
└── .project / .cproject    # Pliki środowiska IDE
```

## Jak uruchomić

1. Otwórz projekt w MCUXpresso IDE (lub STM32CubeIDE)
2. Skonfiguruj pinout dla czujnika, LED-ów i serwa
3. Wgraj kod na płytkę
4. Umieść czujnik naprzeciwko pojazdu (np. modelu auta)
5. Obserwuj reakcję systemu: otwarcie szlabanu, zmiana LED, automatyczne zamknięcie

## Przyszłe usprawnienia

- Licznik wolnych miejsc parkingowych
- Wyświetlacz (np. LCD/I2C) z komunikatem dla kierowcy
- Integracja z systemem RFID do autoryzacji pojazdów

## Autor

Maciej Filipiak – student AGH, pasjonat elektroniki i systemów wbudowanych  
Projekt realizowany w ramach kursu **Techniki Mikroprocesorowe 2**