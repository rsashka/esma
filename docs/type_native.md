# Нативные типы данных

## Системные типы

### :Pointer — указатель на системную область памяти или нативную функцию

Так как любой программе приходится взаимодействовать с внешним миром, то по неволе приходится закладывать возможность использования других библиотек и системы типов данных, и для этих целей служит тип *:Pointer*. Он создается при импорте функций из внешних библиотек и вручную его создать нельзя. Но можно вывести его значение, например для отладки.

### :Plain — указатель на представление данных в бинарном виде

Для взаимодействия с внешними библиотеками требуется еще и обмен данными. И для этих целей служит тип данных *:Plain* — который также является указателем, но на двоичное представление данных в области памяти. Конечно, если их можно представить в виде одного фрагмента.


### Перечисление, структура и объединение

*:Enum*, *:Struct* и *:Union* — это такие же словари, только на их элементы накладываются определённые ограничения. Каждый элемент должен иметь уникальное имя, а его тип данных должен быть простым, т.е. числом или строкой фиксированного размера. Эти типы данных так же относятся к группе *:Plain* и могут быть представлены в двоичном виде в одной области мишинной памяти.

### Обычная функция

Обычная функция — такие функции являются именно обычными функциями в понимании С/С++. Внутри них можно писать совершенно любой код, включая проверки условий, циклы, вызовы других функций и т.д.

Внутри обычной функции можно обращаться к локальным и глобальным объектам, и они могут содержаться вставки на языке реализации*, например, для вызова функций из внешних библиотек.

Вставки на языке реализации оформляются в виде **%{** ... **%}** и могут содержать любой текст на С/С++, а прямо из него можно обращаться к локальным и глобальным объектам NewLang так же, как и в обычном синтаксисе, указывая первым символом имени соответствующий модификатор (*$* для локальных объектов и *@* для глобальных).

Технически, такая программная вставка просто переносится трансплайтером непосредственно в исходный текст генерируемого файла, а все идентификаторы *NewLang* специальным образом декорируются (добавляются специальные маркеры для их идентификации), после этого исходный текст подается на вход обычному компилятору С++. Для локальных объектов трансплайтер может генерировать код для прямого доступа к объекту на этапе компиляции, а для работы с глобальными объектами вынужден использовать runtime вызовы функции поиска в таблице символов.
***
*) — Программные вставки на языке реализации обрабатываются только во время компиляции

Например:
```
  print(str) := { 
      %{ 
          printf("%s", static_cast<const char *>($str)); /* Прямой вызов С функции */ 
      %} 
  };
```

## Чистые функции

Чистая функция — это тоже обычная функция, только в том смысле, какой в него вкладывает функциональное программирование. Создание чистой функции происходит с помощью операторов **:-** или **::-**, а сам оператор заимствован из языка Пролог. У чистой функции отсутствует доступ к контексту и глобальным переменным, поэтому она может обрабатывать только те данные, которые были ей переданы в качестве аргументов.

Программные вставки на языке реализации внутри чистых функций не запрещены и могут использоваться, например, для отладки. Но делается это на страх и риск разработчика. Именно он отвечает за их «чистоту», например при вызове функций из внешних библиотек.

```
  Sum1(arg1, arg2) :- {$arg1+$arg2;}; # Создать или переопределить простую функцию, которая возвращает сумму аргументов
  Sum2(arg1, arg2) ::- {$arg1+$arg2;}; # Тоже самое, но если функция с таким именем уже существует, то будет ошибка
```


### Пусто (:None)

*:None* (пусто) — не содержит значения (точнее имеет одно значение *None*). Указывается в тексте программы как один подчерк «_». Значение *None* имеют не инициализированные переменные и при попытке чтения из такой переменной возникает ошибка.

Тип переменной может быть явно указан или выведен автоматически из присваиваемого значения. Присвоить новое значение уже инициализированной переменной можно только для совместимого типа, так как неявное преобразование типов не допускаются.

```
  $var := _; # Создать не инициализированную переменную
  $var2 := var; # Ошибка!!! Нельзя прочитать неинициализированную переменную var
  $var = 1000; # У переменной будет тип Short (минимальный размер для хранения значения)
  $var = 0,5; # Ошибка!!! Short ← Float не совместимы
  $var = _; # Очистить значение переменной
  $var = 0,5; # Теперь можно, т. к. None совместим с любым типом
```
