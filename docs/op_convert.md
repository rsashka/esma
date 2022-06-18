#### Data comprehensions


https://github.com/obsidianmd/obsidian-docs/blob/master/en/How%20to/Format%20your%20notes.md



Хоть тензоры и являются основным элементом вычислений, но обрабатывать приходится данные различных типов. Для преобразования  типов данных переменных или изменения размерности у тензоров используется отдельная синтаксическая конструкция, которая состоит из двойных квадратных скобок ```[[ данные ]]:Type[dims]```, где *Type* результирующий тип данных, а *dims* количество элементов тензора в каждой размерности, которые перечисляются через запятую. Для тензора с автоматическим выводом типа необходимо указать тип Tensor, а для типов данных отличных от тензоров размерность не указывается. Если тип данных не указан, то результат вычисления будет ??????????????????? одномерный словарь ????????????.

Фактически, данная синтаксическая конструкция является одновременно и оператором времени компиляции и функцией времени выполнения в зависимости от указанных между двойными квадратными скобками значений.

Так, выражение ```[[ var1, var2 ]]:[...]``` будет вычисляться во время выполнения, когда будет известно значение переменных *var1* и *var2*, а результатом её выполнения будет одномерный тензор с автоматическим выводом тип. Для создания тензора с заданным типом данных используется выражение ```[[ varibale ]]:Type```, где *Type* — любой из арифметических типов.

Если требуется получить тензор конкретного типа и заданной размерности, то это делается выражением ```[[ varibale ]]:Type[2,2]```, которая вернет тензор с размерностью 2х2 и типом *Type* у элементов, а для создания скаляра указывается размерность **0**, т.е. ``` [[ scalar ]]:Int[0]```. В последнем случае в скаляр можно преобразовать только другой скляр.

Внутри двойных квадратных скобок через запятую может быть указано несколько выражений, любой литерал или диапазон, значения которых будут объединены и преобразованы в одину переменную.


scalar_int := 100:Int; # Тип скаляра во время компиляции
scalar_int := 100:Int(__device__="GPU"); # Тип скаляра во время компиляции
scalar_int := 100:Int(); ?????? # Тип скаляра во время компиляции
:type_int := :Int; # Синоним типа Int во время выполнения (не может быть изменен)
:type_int := :Int(); # Копия типа Int во время выполнения (может быть изменен после Mutable)
scalar_int := :Int(100); # Преобразование типа во время выполнения

:синоним := :Int; # Неизменяемый ?????
:копия := :Int(); # Изменяемый ?????

Mutable(:синоним); # Ошибка
Mutable(:копия); # Норма

const_(); # Функция объекта
mutable_(); # Функция объекта
Const(); # Функция 
Mutable(); # Функция 

значение := :Int;
значение := :Int();
значение := :Int(1);
значение := :Int(1,2);
значение := :Int[0](1);
значение := :Int[2](1,2);
значение := :Int[...](1,2,3);

100:Int
100:Int(__device__=CPU)
100:Int(__device__=GPU)
100:Int(__shared__=\true) # Константа на этапе компиляции
:Int(100, __shared__=\true) # Любая переменная на этапе выполнения
100:Int(__device__=CPU)
100:Int(device=GPU)

scalar_int := :Int[0](1, 2, 3);
tensor_int_auto := :Int(1, 2, 3);
tensor_int_2_2 := :Int[2,2](1, 2, 3, 4);
tensot_auto_2_2 := :Tensor[2,2](0, 1, 2, 3.333);

:Int(var)    - Изменение типа без изменения размерности
:Int[0](var) - Изменение типа и размерности в скаляр
:Int[1](var) - Тензор  Int с одним элементом
:Int[...](var) - Преобразование типа тензора в и изменение в одно измерение с произвольным количеством элементов
:Tensor[...](var) - Изменение размерности тензора без изменения его типа данных


enum := :Enum(one=1, two=_, "three", ten=10);
struct := :Struct(byte:Char, int:Int=0, str:StrChar[10]=0, array:Double[10]);

++ только для строковых переменных !!!!!!!!!!!!!!!


:Dict := :Dictionary;

array := :Dict(«One», «Two», «Three», «Four»);

[[ :Bool(array) ]] <-> { # Цикл, пока словарь не пустой
    item, array := ... array; # Раскрывает словарь на первый и остальные элементы
    print(«Single item: $item\n»($item)); # Выводим элементы словаря по одному БЕЗ ИТЕРАТОРА И ИНДЕКСНОЙ ПЕРЕМЕННОЙ
}
first, ... := ... array; # Первый элемент
first, _ , three, ... := ... array; # Первый и третий элементы





var := [[  1,name=2,3, name=4, ]]:Class; #Так нельзя, т. к. оператор [[]] раскрывает значения всех  переменных

# аргументы вызова раскрываются ДО вызова и должны быть определены заранее!!!
var := :Class(1, name=2,3, name=4); # Объект заданного класса

# данные в словаре раскрываются ПОСЛЕ вызова и могут быть не определенными на момент вполнения
var := :Class(1, name=2,3, name=4); #Словарь указанного типа данных
Object::Call(Object args ...);

# При созданиеи перечисления данные в словаре раскрываются ПОСЛЕ вызова и могут быть не определенными на момент вполнения
var := [[one:Int=10, two, three=20]]:Enum;
var := (one:Int=10, two, three=20,):Enum;
Object::Compile(TermPtr args ...);
var := [>one:Int=10, two, three=20>]:Enum;
var := <[one:Int=10, two, three=20]>:Enum;

<![CDATA[ и завершается ]]>

<[  ]>
[<  >]
[>  <]

[{  }]
{[  ]}


# фактически, это парсинг и интерпретация кода во время выполнения


:EnumName(one:Int, two, three=10) := :Enum;
:ClassName(name1, name2=_) := :Class; # Объект заданного класса


:Пол(Мужской, Женский) := :Enum;
:Обезьяна(пол:Пол) := :Class;
:Человек(родители:=(,)) := :Обезьяна;

:Пол := :Enum(Мужской, Женский);
:Обезьяна := :Class(пол:Пол);
:Человек := :Обезьяна(родители:=(,));
:Человек2 := :Обезьяна(родитель1:Человек, родитель2:Человек);


:Обезьяна := :Class(пол:Пол);
:Человек := (родители:Человек[...]=(,),):Обезьяна;
:Человек2 := (родитель1:Человек, родитель2:Человек,):Обезьяна;

tensor:Tensor[5, 10] — тензор с заданными размерами ??????
tensor:Tensor[5, ...] — тензор с заданными размерами ??????
tensor:Int[...] — тензор заданного типа с произвольными размерами ??????
dict:Dictionary[...] — словарь с элементами указанных типов??????
enum:Enum — элемент указанного типа (перечисления)
dict:Class — элемент типа словарь
dict:Enum[...] — словарь с элементами указанного перечисления??????
dict:Class[...] — словарь с элементами указанных типов??????
dict:Class[1..3] — словарь с 1,2 или 3 элементами указанных типов??????





###### Примеры:
```[[0]]:Int[1, ...]``` - Скаляр

[[0]]:Int - Тензор Скаляр сокращенная запись без изменения размерности ??????????????????? [[0]]:Int[] - !!!!!!!  Использовать нельзя т.к. пересекается с добавлением эелеменат в массив и пропущена размерность тенора (т.е. для признака тензора будет не достаточно размерности и потребуется хранить отдельный флаг) [[0]]:Int[...] - Тензор с одним измерением и произвольным числом элементов в нем[[ var ]]:Int    - Изменение типа без изменения размерности
[[ var ]]:Int[0] - Изменение типа и размерности в скаляр
[[ var ]]:Int[1] - Тензор  Int с одним элементом
[[ var ]]:Int[...] - Преобразование типа тензора в и изменение в одно измерение с произвольным количеством элементов
[[ var ]]:Tensor[...] - Изменение размерности тензора без изменения его типа данных```

    > tstr := :Int[0]("Тест")   # Создать тензор из строки широких символов
    > tstr := [["Тест"]]:Int[0]   # Создать тензор из строки широких символов
    [1058, 1077, 1089, 1090,]:Int

    > t2 := :Int[2,2]("Тест") # Тоже самое, но тензор двухмерный
    > t2 := [[ "Тест" ]]:Int[2,2] # Тоже самое, но тензор двухмерный
    [
      [1058, 1077,], [1089, 1090,],
    ]:Int

    > :StrWide(tstr) # Конвертировать тензор обратно в строку
    Тест

    > :Double(t2)    # Изменить тип данных тезора
    [
      [1058, 1077,], [1089, 1090,],
    ]:Double

    > t3 := :Char[4](t2) # Преобразовать тип данных тензора и его размерность
    > t3 := [[ t2 ]]:Char[4] # Преобразовать тип данных тензора и его размерность
    [34, 53, 65, 66,]:Char



Чтобы преобразовать любые данные в тензор (с учетом допустимости такого преобразования), её достаточно указать между двойными квадратными скобками и через двоеточие указать тип данных и/или размерность тензора. 


Внутри двойных квадратных скобок может быть не только любое выражение, но и литерал или диапазон. В этом случае, они также раскрываются в тензор по таким же правилам. В будущем планирую добавить возможность указания сразу нескольких значений через запятую для их объединения в один тензор.

#### Преобразование тензоров
В эпоху машинного обучения тензоры являются основным элементом вычислений, поэтому для конвертирования данных в тензоры
используется отдельная синтаксическая конструкция, состоящая из двойных квадратных скобок **[[** *данные* **]]**.
Подробнее про особенности преобразования типов можно посмотреть в [полном описании синтаксиса](https://github.com/rsashka/newlang/blob/master/Syntax.md).
  
Указание типа для скаляра (разменость 0)
размерности указываются через запятую
:Int[0](0);  [[0]]:Int[0] - Скаляр
:Int[1, ...](0) [[0]]:Int[1, ...] - Скаляр
:Int(0)  [[0]]:Int - Тензор Скаляр сокращенная запись без изменения размерности ???????????????????
:Int[](0) [[0]]:Int[] - !!!!!!!  Использовать нельзя т.к. пересекается с добавлением эелеменат в массив и пропущена размерность тенора 
(т.е. для признака тензора будет не достаточно размерности и потребуется хранить отдельный флаг)
:Int[...](0) [[0]]:Int[...] - Тензор с одним измерением и произвольным числом элементов в нем

[[ var ]]:Int    - Изменение типа без изменения размерности
[[ var ]]:Int[0] - Изменение типа и размерности в скаляр
[[ var ]]:Int[1] - Тензор  Int с одним элементом
[[ var ]]:Int[...] - Преобразование типа тензора в и изменение в одно измерение с произвольным количеством элементов
[[ var ]]:[...] - Изменение размерности тензора без изменения его типа данных


