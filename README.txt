Instrukcja używania aplikacji:

Aplikacja posiada dwa tryby: interaktywny oraz automatyczny

Aby uruchomić ją w trybie interaktywnym należy wywołać ją bez argumentów - bezpośrednio z pulpitu bądź z terminala.

W trybie interaktywnym będziemy w pętli pytani o algorytm który chcemy wywołać. Po wyborze algorytmu (liczby 1-8 i wciśnięcie ENTER) będziemy poproszeni o wpisanie ściezki do grafu(grafów) na których chcemy wywołać algorytm.
Dla uproszczenia używania dodany zostal mechanizm który pozwala na proste użycie wcześniej wpisanych grafów. Za każdym razem gdy będziemy proszeni o ścieżkę do grafu będziemy widzieć listę do tej pory wpisanych grafów. Wystarczy wpisać numer tego gradu aby uruchomić go.

Tryb automatyczny:

Jeżeli wywołamy naszą aplikację z parametrami zostanie wywołany tryb automatyczny.

<plik wykonywalny> nazwa_pliku_wyjsciowego nazwa_pliku_z_grafem_1 [nazwa_pliku_z_grafem_2] 

Jeżeli nie podamy drugiego grafu to wywołane zostaną jedynie algorytmy wymagające jednego grafu (rozmiary, kliki). Jeśli podamy oba to wywołane zostaną również subgrafy i metryka.

Wynik będzie zapisany w pliku podanym na wejściu. Po zakończeniu program od razu się wyłączy.

Kompilacja:

Opcja pierwsza - otworzyć folder z kodami źródłowymi używając Visual Studio z zainstalowanym dodatkiem do cmake i użyć standardowej kompilacji.

Opcja druga - należy mieć zainstalowany cmake oraz kompilator c++ i należy mieć poprawnie ustawioną zmienną środowiskową PATH. Następnie należy wywołać następujące komendy:

cmake.exe --build ${OUTPUT} --target taio_console

Gdzie output to miejsce gdzie chcemy otrzymać skompilowany projekt.