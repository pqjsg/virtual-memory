# virtual-memory
Лабораторная работа. Университетское задание.

Реализовать класс для управления виртуальной памятью. Объект этого класса моделирует массив  целого типа очень большой размерности (>10000 элементов).
При создании объекта инициализируется файл подкачки (двоичный файл прямого доступа). Файл содержит сигнатуру - два байта, представляющие символы ‘ВМ’, и блоки (страницы) размерности 512 байт, Каждая страница состоит из элементов, соответствующих ячейкам моделируемого массива, и содержащих байты значений целого типа. Каждой странице предшествует битовая карта (массив байтов), в которой каждый бит соответствует ячейке моделируемого массива, находящейся на странице. Значение бита 0 означает, что в эту ячейку ничего не записано.
Количество страниц определяется путём выравнивания суммарного объема памяти моделируемого массива на границу страницы. 
