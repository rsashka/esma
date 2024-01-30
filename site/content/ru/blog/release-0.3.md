---
slug: release-v0.3
title: Релиз языка программрования NewLang 0.3
date: 2022-11-07
---

![КДПВ](/ru/blog/langs.jpeg)


{{% pageinfo %}}

Внимание!!!

Данная статья содержит описание синтаксиса NewLnag предыдущей версии.

Актуальную версию синтаксиса языка можно посмотреть [тут](/ru/docs/).

{{% /pageinfo %}}


NewLang — это язык программирования высокого уровня общего назначения. Основной особенностью языка является простой, логичный и не противоречивый синтаксис, который основан на строгой системе грамматических правил. За счет использования макросов, синтаксис языка легко расширяется до более привычного вида на основе ключевых слов. 

NewLang находится в процессе активного развития. Это третий публичный [релиз](https://github.com/rsashka/newlang/releases/tag/v0.3.0) в который добавлены новые существенные возможности по сравнению с предыдущей версией.  


## Зачем нужен *NewLang*?
У всех современных языков программирования происходит постоянное развитие (усложнение) синтаксиса по мере выхода новых версий. Это является своего рода платой за появление новых возможностей и воспринимается пользователями как естественное явление.

Но одновременно является и серьезной проблемой, так как с выходом версий добавляются новые ключевые слова и синтаксические конструкции, что неизбежно повышает порог входа для новых пользователей. Еще одним следствием этого процесса становится постоянное повышение сложности разработки и поддержки уже созданных программных продуктов, когда старый код дорабатывается с применением уже новых стандартов.

У *NewLang* сложность языковых конструкций ограничена естественным образом за счет разделения синтаксиса языка на две части - это упрощает его изучение и использование.

*Основной синтаксис* — для написания программ в объектно-ориентированном (императивном) и декларативном стилях, который основан не на зарезервированных ключевых словах, а на системе строгих [грамматических правил](/ru/docs/syntax/). Имеется возможность расширения основного синтаксиса за счет использования макросов. *Расширенный синтаксис* — программные вставки на языке реализации (С/С++), когда основного синтаксиса становится недостаточно.

Еще одно неудобство современных языков в том, что большинство из них были созданы до начала эпохи машинного обучения, поэтому тензорные вычисления у них выполнены в виде отдельных библиотек. Это же касается и вычислений с неограниченной точностью, которые так же требуют применения дополнительных библиотечных функций.

У *NewLang* тензорные вычисления и рациональные числа неограниченной точности доступны «из коробки». Они поддерживаются на уровне синтаксиса для записи литералов соответствующих типов, а простые арифметические типы данных являются скалярами (тензорами нулевой размерности). Реализация тензорных вычислений сделана на базе библиотеки [libtorch](https://pytorch.org/), а рациональные числа с использованием [OpenSSL](https://github.com/openssl/openssl/blob/master/crypto/bn/README.pod).


## Новые глобальные фичи и изменения в синтаксисе:

### Простые чистые функции удалены
Раньше я пытался использовать синтаксис предикатов из Пролога, в котором проверки условий записываются через запятую. Но эта идея оказалась провальной, а сами операторы просто ужасными (тестировал разные варианты, но каждый раз получал много отрицательных отзывов), поэтому сейчас решил полностью от них отказаться. 

Они получались не только сложными для восприятия, но и по факту не нужными, так как легко заменялись обычной функцией с комбинации обычных логических операций.

### Зафиксирован синтаксис операторов проверки [условия](/ru/docs/ops/if/) и [циклов](/ru/docs/ops/while/).
Изменен и упрощен синтаксис операторов проверки условия (импликации) и цикла, для которых оставлено только по одному варианту. Проверка условия записывается в виде оператора математического следования  **-->**, а цикл с помощью оператора **<->**. 

Но для упрощения записи можно пользоваться макросами, тогда в этом случае операции проверки условий и циклы превращаются в классические 
```
\if(...){
    ...
} \elif(...) {
   ...
} \else {
   ...
};
```
или
```
 \while( ... ){ 
     ...
}; 
```

### Конструкция else у операторов цикла
Теперь оператор цикла **while** поддерживает конструкцию [**else**](/ru/docs/ops/while/), которая выполняется если условие входа в цикл не было выполнено. *Это поведение отличается от аналогичных конструкций  в языке Python, у которого секция **else** выполняется всегда, кроме прерывания цикла по break.*  

Ветка **else** у оператора цикла записывается так же как и ветка иначе в условном операторе, т. е. 
```
[ cond ] <-> {
   ...
}, [_] --> {
   ...
};
```
Или тоже самое, но с использованием макросов:
```
\while(cond) {
   ...
} \else {
   ...
};
```

### Пространства имен
В синтаксис NewLang добавлены [пространства имен](/ru/docs/syntax/namespace/), в стиле очень похожим на С++. Имена разделяются двойным двоеточием, а для определение пространства имен, его нужно указать перед открывающейся фигурной скобкой. 
```
ns {
    name {
        var := 0; # Имя переменной будет ns::name::var
        ::var := 1; # Переменная из глобального пространства имен
    }
}
```

### Программные модули
Реализована концепция программных модулей - которая повторяет концепцию иерархического расположения файлов в структуре каталогов файловой системы, как в языках Python и Java. 

Имя модуля начинается на префикс **@**, а структура каталогов указывается через точку. Причем концепции программных модулей и пространства имен объединены, и полное имя переменой из предыдущего абзаца будет `@root.dir.module::ns::name::var`, где **root** и **dir** это каталоги в файловой системе, а **module** — имя файла.

### Объектно ориентированное программирование
Реализована часть концепции ООП и добавлена поддержка [определения классов](/ru/docs/types/class/) и их наследование, которая сейчас выглядит следующим образом:
```
:NewClass := :Class() { #  Новый тип (класс)
    field := 1;
    method() := {};
};
obj := :NewClass(); # Экземпляр класса
```
Неожиданно для самого себя понял, что имея полный набор вариантов проверок при создании объектов (**::=** - создать новый объект, **:=** - создать новый или присвоить значение существующему, **=** - только присвоить значение, а если объект не существует будет ошибка), концепция переопределения наследуемых функций не требует вообще никаких ключевых слов:
```
:NewClass2 := :NewClass() { # Новый класс на базе существующего
    field ::= 2; # Будет ошибка, т. к. поле field уже есть в базовом классе
    method() = {}; # Аналог override, т.к. method должен существовать в базовом классе
};
```

### Прерывания, возврат и обработка ошибок
Изменена, а точнее полностью переделана идеология возвратов из функций и обработки ошибок. 
Теперь она чем-то похожа на подход, примененный в Ruby. Любая последовательность команд заключенные в фигурные скобки (в том числе тело функции), рассматривается как блок кода у которого нет специального оператора аналога **return**, который возвращает какое либо значение. 
Просто любой блок кода всегда возвращает последнее вычисленное значение (это чем то похоже на оператор «запятая» в языках C/C++).

Для того, чтобы прервать выполнение кода используются две разные команды - прерывания, которые условно можно назвать *положительным* и *отрицательным* результатом. Что примерно соответствует семантике их записи. 
"Отрицательное" прерывание записывается в виде двух минусов, а "положительное" прерывание в виде двух плюсов, т.е. **-\-** или **++**.

По умолчанию данные операции возвращают пустое значение. Чтобы прерывание вернуло результат, возвращаемые данные нужно записывать между парой соответствующих символов, 
т.е. **-\-** 100 **-\-**, что является близким аналогом оператора `return 100;` в других языках программирования, а **++**«Строка»**++** - аналогом `return «Строка»;`.

Хотя более точным аналогом этих операторов будет все таки не **return**, а **throw**, 
т.к. эти команды не только прерывают выполнение последовательности команд в блоке, но их еще можно «ловить». 
Для этого используется блок кода с соответствующей семантикой, **{+** … **+}** - блок кода, 
который перехватывает положительные прерывания и **{-** … **-}** - блок кода, который перехватывает прерывания, созданные операторами **--**. 

Подобная концепция (в явном виде не разделять возвраты из функций и обработку исключений), 
хоть и выглядит немного необычной, но позволяет реализовывать несколько очень полезных финтов, 
которые сложно реализуемые в обычных языках программирования.

Например, возврат из нескольких вложенных функций без необходимости обрабатывать результат возврата каждой из них. 
В этом примере функция *Test* перехватывает "положительные" прерывания из вложенных функций:
```
    Test0(arg) := { \if($arg==0) \return("DONE - 0"); «FAIL» };
    Test1(arg) := { \if($arg==1) \return("DONE - 1"); Test0($arg); };
    Test(arg) := {+ \if($arg >= 0) Test1($arg); $arg; +};

    Test(0); # Вернет «DONE — 0» возврат из вложенной функции Test0
    Test(1); # Вернет «DONE — 1» возврат из вложенной функции Test1
    Test(2); # Вернет «FAIL» возврат из вложенной функции Test0
    Test(-2); # Вернет -2 — возврат из функции Test
```
Есть еще блок `{* … *}`, который перехватывает оба типа прерываний. Такой блок кода поддерживает типизацию возвращаемого значения, что позволяет в явном виде указывать типы данных, которые нужно перехватывать. 
Например, `{* ... *} :Type1` — будет перехвачено прерывание вида `++ :Type1 ++` или `--:Type1--`, что позволяет очень гибко формировать логику работы программы.

Блоки кода с перехватом исключений также поддерживают оператор иначе (*\else*) который, по аналогии с оператором *\else* в циклах, выполняется только тогда, если прерывания не произошло.

Можно указать сразу несколько типов, которые нужно перехватывать:
```
    {* 
        ....
    *} <:Type1, :Type2, :Type3>;
```

## Бинарная сборка под Windws:
В текущем релизе версия clang повышена 15, а вызовы нативных функций опять реализованы с помощью libffi, что в итоге позволило собрать бинарную сборку не только под Linux, но и под Windows. И теперь чтобы поиграться с REPL read-eval-print loop можно скачать уже готовый бинарник.


## Подробная информация о языке:
- [Синтаксис *NewLang*](/ru/docs/syntax/)
- [Подробное описание типов данных](/ru/docs/types/)
- [Операторы и управляющие конструкции](/ru/docs/ops/)
- [Вся документация по *NewLang* на одной странице](/ru/docs/_print/)

## Обратная связь
Если у вас появятся предложения по развитию нового или улучшению уже существующего функционала *NewLang*, [пишите](https://github.com/rsashka/newlang/discussions).


## Примеры кода

```
#!../output/nlc --eval
# Определение функции hello
hello(str) := { 
    # Импорт стандартной C функции
    printf := :Pointer('printf(format:FmtChar, ...):Int32');
    # Вызов C функции с проверкой типов аргументов по строке формата
    printf('%s', $str);  
    # Возврат значения из функции hello
     $str;
};

hello('Привет, мир!'); # Вызвать функцию</code>
```
Вывод (первая строка выводится с помощью printf, а вторая — возвращаемое значение функции hello):
```
    Привет, мир!
    Привет, мир!
```
Пример скрипта для вычисления факториала 1000 
```
#!../output/nlc --eval

fact := 1\1;  # Рациональное число без ограничения точности
mult := 1000..1..-1?; # Сделать из диапазона итератор для множителей от 1000 до 2
[mult ?!] <-> {      # Цикл, пока не закончатся данные итератора
    # Получить текущий множитель и перейти на следующий элемент итератора
    fact *= mult !; 
};
fact # Вывести итоговый результат
```
Вывод:

<spoiler title="Много цифр">
402387260077093773543702433923003985719374864210714632543799910429938512398629
020592044208486969404800479988610197196058631666872994808558901323829669944590
997424504087073759918823627727188732519779505950995276120874975462497043601418
278094646496291056393887437886487337119181045825783647849977012476632889835955
735432513185323958463075557409114262417474349347553428646576611667797396668820
291207379143853719588249808126867838374559731746136085379534524221586593201928
090878297308431392844403281231558611036976801357304216168747609675871348312025
478589320767169132448426236131412508780208000261683151027341827977704784635868
170164365024153691398281264810213092761244896359928705114964975419909342221566
832572080821333186116811553615836546984046708975602900950537616475847728421889
679646244945160765353408198901385442487984959953319101723355556602139450399736
280750137837615307127761926849034352625200015888535147331611702103968175921510
907788019393178114194545257223865541461062892187960223838971476088506276862967
146674697562911234082439208160153780889893964518263243671616762179168909779911
903754031274622289988005195444414282012187361745992642956581746628302955570299
024324153181617210465832036786906117260158783520751516284225540265170483304226
143974286933061690897968482590125458327168226458066526769958652682272807075781
391858178889652208164348344825993266043367660176999612831860788386150279465955
131156552036093988180612138558600301435694527224206344631797460594682573103790
084024432438465657245014402821885252470935190620929023136493273497565513958720
559654228749774011413346962715422845862377387538230483865688976461927383814900
140767310446640259899490222221765904339901886018566526485061799702356193897017
860040811889729918311021171229845901641921068884387121855646124960798722908519
296819372388642614839657382291123125024186649353143970137428531926649875337218
940694281434118520158014123344828015051399694290153483077644569099073152433278
288269864602789864321139083506217095002597389863554277196742822248757586765752
344220207573630569498825087968928162753848863396909959826280956121450994871701
244516461260379029309120889086942028510640182154399457156805941872748998094254
742173582401063677404595741785160829230135358081840096996372524230560855903700
624271243416909004153690105933983835777939410970027753472000000000000000000000
000000000000000000000000000000000000000000000000000000000000000000000000000000
000000000000000000000000000000000000000000000000000000000000000000000000000000
000000000000000000000000000000000000000000000000000000000000000000000000\1
</spoiler>

## Как посмотреть?
- [Бинарная сборка и тесты под Windows](https://github.com/rsashka/newlang/releases/download/v0.3.0/nlc_win_64.zip) 
- [Бинарная сборка и тесты под Ubuntu](https://github.com/rsashka/newlang/releases/download/v0.3.0/nlc_lin_64.tar.xz) 
- Для запуска бинарных сборок потребуются разделяемые библиотеки libLLVM-15 и libtorch ([архив с библиотеками для Windows](https://github.com/rsashka/newlang/releases/download/v0.3.0/nlc_dll.zip)), ([архив с библиотеками для Ubuntu](https://github.com/rsashka/newlang/releases/download/v0.3.0/nlc_so.tar.xz)).
- Так же все можно собрать напрямую из исходников по инструкции в [репозитории](https://github.com/rsashka/newlang)