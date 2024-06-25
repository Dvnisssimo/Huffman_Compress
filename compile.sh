gcc -o Huffman_Compress source/Huffman_Compress.c

if [ $? -eq 0 ]; then
    echo "Компиляция завершена успешно. Исполняемый файл: Huffman_Compress"
    chmod +x Huffman_Compress
    if [ -f Huffman_Compress ] && [ -x Huffman_Compress ]; then
        echo "Исполняемый файл создан и имеет права на выполнение."
    else
        echo "Ошибка: Исполняемый файл не найден или не имеет права на выполнение."
    fi
else
    echo "Ошибка компиляции."
fi