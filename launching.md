
# Запуск программы

## Компиляция

## Выполнение

```
./result in.txt out.txt
```

## Формат входных данных

На вход кодера подаётся несжатый файл произвольного расширения, битовое представление которого используется для дальнейшего сжатия (упаковки).

На вход декодера подаётся список троек *⟨offset, length, next⟩* для дальнейшей распаковки.

## Формат выходных данных

Результатом кодирования является список структур (троек) следующего вида: 
```
struct Node:
    int offset
    int length
    char next
```