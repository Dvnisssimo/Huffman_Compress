# Архиватор на основе алгоритма Хаффмана

## Инструкция по использованию
1) Запускаем встроенную в Windows командную строку
2) В командной строке переходим в директорию где лежит проект
> cd "путь к папке с проектом"
3) Используем команду для запуска .exe файла со следующими аргументами
> ./Huffman_Compress <Команда> <"Путь к исходному файлу с указанием расширения"> <"Путь сохранения обработанного файла с указанием расширения">
>> Список команд описан ниже, расширение обработанного файла вы выбираете сами (можно задать любое)
4) Далее происходит обработка файла и после успешного сжатия он будет сохранен в указанной вами директории

## Список команд
1) encode - Сжимает указанный файл
2) decode - Распаковывает указанный файл

## Пример использования
Выполним сжатие и распаковку файла

В данном проекте уже есть текстовый документ для теста, обработаем его

Следуем инструкции...

В моем случае проект находится на рабочем столе
> cd C:\Users\Dvnissimo\Desktop\Huffman_Compress

Сжимаем тестовый файл
> ./Huffman_Compress encode test.txt test.zip

В данном случае я не указывал полный путь до файлов, так как исходный файл находится в корневой папке с проктом, и сохраним обработанный файл туда же

Файл был сжат с 19642 КБ до 6846 КБ

С операцией распаковки все то же самое
> ./Huffman_Compress decode test.zip test.txt

Готово!

## Компиляция .exe
1) Устанавливаем Cygwin64 Terminal с пакетами gcc
2) Открываем этот терминал и переходим в директорию с нашем проектом
> cd C:/Users/Dvnissimo/Desktop/Huffman_Compress
3) Указываем команду для запуска файла компиляции
> bash compile.sh
