Сборка: `autoreconf -fisv && ./configure --prefix=LOCATION && make`, где `LOCATION` это, например, `/usr` или `$PWD`

Для установки в `LOCATION/bin` необходимо выполнить команду `make install`. Для удаления `make uninstall`.

Юнит-тесты могут быть запущены с помощью `make check`.

Для просмотра HTTP-страницы с документацией достаточно команды `make http`. Просмотреть man-страницу можно с помощью `man LOCATION/doxygen-doc/man/man1/hangman.1`.

Поддерживается запуск игры в русской локали и с кастомным списком слов(для этого необходимо передать путь к файлу wordlist.txt с помощью опции `-d`).

Список слов был взят из https://github.com/Xethron/Hangman/blob/master/words.txt