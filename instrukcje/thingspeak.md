# ThingSpeak - opis

ThingSpeak to serwis, który pozwala na gromadzenie danych pomiarowych z różnych czujników oraz tworzenie wykresów do zgromadzonych danych. Wykresy oraz pomiary mogą być prywatne lub publicznie dostępne. Serwis wymaga rejestracji. Ma darmowe konta ograniczone do 3 mln pomiarów na rok. Więcej info w [FAQ](https://thingspeak.com/pages/license_faq) i na stronie [ThingSpeak.com](https://thingspeak.com). ThingSpeak pozwala na dostęp do danych pomiarowych z dowolnego miejsca. Wystarczy tylko login i hasło. Jeśli chcesz mieć dostęp do informacji ze swojego Smogomierza w każdym miejscu, to ThingSpeak powinien w tym pomóc. Dodatkowo serwis posiada aplikacje dostępne na smartfony. Jest to wygodne rozwiązanie dla osób, które nie chcą tworzyć własnej bazy danych, a zależy im głównie na dostępnie do informacjach z własnego miernika.

##  Rejestracja

1. Po wejściu na stronę [ThingSpeak.com](https://thingspeak.com), klikamy przycisk "Sign Up"(prawy górny róg strony). 

2. Podajemy nasz email, login oraz hasło, wybieramy kraj oraz podajemy imię i nazwisko.
![ThingSpeak1](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/screens/ThingSpeak1.jpg)

3. Po rejestracji i zalogowaniu się na nasze nowe konto, musimy utworzyć nowy kanał. Na każdy kanał możemy wysyłać 8 równych pomiarów. W naszym przypadku wystarczy 1 kanał, gdyż będziemy wysyłać 6 pomiarów(PM1, PM2.5, PM10, Temperatura, Wilgotność i Ciśnienie). W celu stworzenia nowego kanału klikamy na zielony przycisk "Channels". a następnie na zielone "New Channel".
![ThingSpeak2](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/screens/ThingSpeak2.jpg)

4. Wypełniamy pole Name oraz pola Field od 1 do 6. Podobnie jak na screenie powyżej. Możemy również podać wysokość w metrach oraz położenie naszego miernika. W tym celu musimy uzupełnić pole Elevation oraz Latitude i Longitude. Na koniec klikamy na zielony przycisk "Save Channel" na dole strony.

5. Po stworzeniu nowego kanału możemy dodać opisy do wykresów. W tym celu klikamy na symbol ołówka nad każdym wykresem i w polu "Title" wpisujemy nazwę wykresu.
![ThingSpeak3](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/screens/ThingSpeak3.jpg)

6. Kolejnym krokiem jest wybranie karty "API Keys". Potrzebujemy z niej pobrać dwa bardzo ważne ciągi znaków: "Channel ID" oraz "Key". Te dwie wartości są unikalne dla każdego użytkownika i będziemy je musieli ustawić w pliku config.h naszego miernika, jeśli chcemy aby nasze pomiary były wizualizowane na wykresach.
![ThingSpeak4](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/screens/ThingSpeak4.jpg)

7. Pozostała nam jeszcze jedna rzecz do zrobienia na stronie ThingSpeak - ustawienie kwestii prywatności. Przechodzimy do karty "Sharing". Domyślnie zaznaczona opcja to "Keep channel view private". Oznacza ona, że dane przesłane do serwisu ThingSpeak będą widoczne tylko nam. Oznacza to również, że wykresy na naszym mierniku smogu będzie widzieć tylko osoba zalogowana na nasze konto w serwisie ThingSpeak. Jeśli jesteśmy jedyną osobą, która ma mieć dostęp do miernika, to nie musimy niczego zmieniać, ale jeśli będziemy chcieli pokazywać nasze dane pomiarowe innym osobom, warto zmienić ten parametr na "Share channel view with everyone". Dzięki temu nawet, gdy się wylogujemy z serwisu ThingSpeak, lub ktoś inny będzie chciał zobaczyć pomiary naszego miernika, będzie mógł to zrobić. Po zaznaczeniu opcji "Share channel view with everyone", warto również przejść do karty "Public View" i ponownie ustawić opisy wykresów(podobnie jak w punkcie 5.).
![ThingSpeak5](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/screens/ThingSpeak5.jpg)

Gotowe! Nowy kanał w serwisie ThingSpeak jest już w pełni gotowy do obsługi naszego miernika smogu. Pozostało jedynie przepisać "Channel ID" oraz "Key" do pliku konfiguracyjnego config.h. Jak to zrobić? Zajrzyj do [Instrukcja instalacji wymaganego oprogramowania](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/software.md) 
