## Реализация утилит cat и grep на языке программирования С
Директория data-samples - Тесты для каждой из утилит

build: make all

Для проверки каждой из реализованных утилит используются Bash скрипты test.sh

Для MacOS и Linux вывод может различаться. Для MacOS можно использовать утилиты gcat и ggrep

Примеры запуска:

./s21_cat "filename"

./s21_grep "substring" "filename"