# snakeGL
Projekt na programowanie obiektowe (2 semestr).
Kiedys to bedzie 3d snake replica.
Poza plikami w repo dolaczyc do projektu nalezy takze foldery z glfw i glew.
Peace.

Wymagania:
OpenGL 4.4 lub wyżej
VC++ redistributable (jeśli nie zainstalowane wyszkoczy błąd o braku runtime dll)

Link do pobrania paczki z vc_redist - https://www.microsoft.com/en-us/download/details.aspx?id=48145

Kompilator -> MSVC

Wykorzystane biblioteki zewnetrzne:
- Glew (do zarzadzania GL-em)
- Glfw (do zarzadzania oknem)
- Soil2 (do ladowania obrazow)
- Glm (biblioteka matematyczna)

Snake model concept:
![](snookConcept.png)

Current game state of preparation:
![](currentGameState.png)

I hate C++, my dudes, but i will finish this one project. I guess...
![](https://i.kym-cdn.com/entries/icons/original/000/028/021/work.jpg)

Todo:
- punktacja
- menu, a w nim:
  - scoreboard z najlepszymi wynikami
  - settingz:
    - zmiana tekstur
    - zmiana koloru czyszczenia bufora
    - glosnosc muzyki
- muzyka
