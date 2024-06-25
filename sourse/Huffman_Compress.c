#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

struct head
{
    int a; //символ
    long int count; //количество раз появления символа в файле
    long int parent, left, right; //дерево
    char bits[256]; //код хаффмана
};

struct head header[512], tmp; //дерево

//Функция читает содержимое файла, сжимает его и записывает сжатое содержимое в другой файл
int compress(const char* filename, const char* output_file)
{
    char buf[512];
    unsigned char c;
    long int i, j, m, n, f, min1, pt1, flength;
    FILE* ifp, * ofp;
    int per = 10;
    ifp = fopen(filename, "rb");
    if (ifp == NULL)
    {
        printf("Ошибка открытия исходного файла: %s\n", filename);
        return 0;
    }
    ofp = fopen(output_file, "wb");
    if (ofp == NULL)
    {
        printf("Ошибка создания сжатого файла: %s\n", output_file);
        return 0;
    }
    flength = 0;
    while (!feof(ifp))
    {
        fread(&c, 1, 1, ifp);
        header[c].count++;
        flength++;
    }
    flength--;
    header[c].count--;
    for (i = 0; i < 512; i++)
    {
        if (header[i].count != 0)
        {
            header[i].a = (unsigned char)i;
        }
        else
        {
            header[i].a = -1;
        }
        header[i].parent = -1;
        header[i].left = header[i].right = -1;
    }

    for (i = 0; i < 256; i++)
    {
        for (j = i + 1; j < 256; j++)
        {
            if (header[i].count < header[j].count)
            {
                tmp = header[i];
                header[i] = header[j];
                header[j] = tmp;
            }
        }
    }

    for (i = 0; i < 256; i++)
    {
        if (header[i].count == 0)
        {
            break;
        }
    }

    n = i;
    m = 2 * n - 1;
    for (i = n; i < m; i++)
    {
        min1 = 999999999;
        for (j = 0; j < i; j++)
        {
            if (header[j].parent != -1)
            {
                continue;
            }
            if (min1 > header[j].count)
            {
                pt1 = j;
                min1 = header[j].count;
                continue;
            }
        }
        header[i].count = header[pt1].count;
        header[pt1].parent = i;
        header[i].left = pt1;
        min1 = 999999999;
        for (j = 0; j < i; j++)
        {
            if (header[j].parent != -1)
            {
                continue;
            }
            if (min1 > header[j].count)
            {
                pt1 = j;
                min1 = header[j].count;
                continue;
            }
        }
        header[i].count += header[pt1].count;
        header[i].right = pt1;
        header[pt1].parent = i;
    }

    for (i = 0; i < n; i++)
    {
        f = i;
        header[i].bits[0] = 0;
        while (header[f].parent != -1)
        {
            j = f;
            f = header[f].parent;
            if (header[f].left == j)
            {
                j = strlen(header[i].bits);
                memmove(header[i].bits + 1, header[i].bits, j + 1);
                header[i].bits[0] = '0';
            }
            else
            {
                j = strlen(header[i].bits);
                memmove(header[i].bits + 1, header[i].bits, j + 1);
                header[i].bits[0] = '1';
            }
        }
    }

    fseek(ifp, 0, SEEK_SET);
    fseek(ofp, 8, SEEK_SET);
    buf[0] = 0;
    f = 0;
    pt1 = 8;

    printf("Файл, который нужно сжать (%d символов): %s\n", flength, filename);
    printf("Сжатие...\n");

    while (!feof(ifp))
    {
        c = fgetc(ifp);
        f++;
        for (i = 0; i < n; i++)
        {
            if (c == header[i].a)
            {
                break;
            }
        }
        strcat(buf, header[i].bits);
        j = strlen(buf);
        c = 0;
        while (j >= 8)
        {
            for (i = 0; i < 8; i++)
            {
                if (buf[i] == '1')
                {
                    c = (c << 1) | 1;
                }
                else
                {
                    c = c << 1;
                }
            }
            fwrite(&c, 1, 1, ofp);
            pt1++;
            strcpy(buf, buf + 8);
            j = strlen(buf);
        }
        if (100 * f / flength > per)
        {
            printf("%d%%\n", per);
            per += 10;
        }
        if (f == flength)
        {
            break;
        }
    }
    printf("100%%\n");

    if (j > 0)
    {
        strcat(buf, "00000000");
        for (i = 0; i < 8; i++)
        {
            if (buf[i] == '1')
            {
                c = (c << 1) | 1;
            }
            else
            {
                c = c << 1;
            }
        }

        fwrite(&c, 1, 1, ofp);
        pt1++;
    }

    fseek(ofp, 0, SEEK_SET);
    fwrite(&flength, 1, sizeof(flength), ofp);
    fwrite(&pt1, sizeof(long int), 1, ofp);
    fseek(ofp, pt1, SEEK_SET);
    fwrite(&n, sizeof(long int), 1, ofp);
    for (i = 0; i < n; i++)
    {
        tmp = header[i];

        fwrite(&(header[i].a), 1, 1, ofp);
        pt1++;
        c = strlen(header[i].bits);
        fwrite(&c, 1, 1, ofp);
        pt1++;
        j = strlen(header[i].bits);

        if (j % 8 != 0)
        {
            for (f = j % 8; f < 8; f++)
            {
                strcat(header[i].bits, "0");
            }
        }

        while (header[i].bits[0] != 0)
        {
            c = 0;
            for (j = 0; j < 8; j++)
            {
                if (header[i].bits[j] == '1')
                {
                    c = (c << 1) | 1;
                }
                else
                {
                    c = c << 1;
                }
            }
            strcpy(header[i].bits, header[i].bits + 8);
            fwrite(&c, 1, 1, ofp);
            pt1++;
        }

        header[i] = tmp;
    }
    fclose(ifp);
    fclose(ofp);

    printf("Успешное сжатие файла!\n");
    printf("\n");

    return 1;
}


//Функция разархивирует файл и записывает его содержимое в новый файл
int uncompress(const char* filename, const char* outputfile)
{
    char buf[255], bx[255];
    unsigned char c;
    char out_filename[512];
    long int i, j, m, n, f, p, l, flength;
    int per = 10;
    int len = 0;
    FILE* ifp, * ofp;
    char c_name[512] = { 0 };

    ifp = fopen(filename, "rb");
    if (ifp == NULL)
    {
        return 0;
    }

    if (outputfile)
    {
        strcpy(out_filename, outputfile);
    }
    else
    {
        strcpy(out_filename, c_name);
    }

    ofp = fopen(out_filename, "wb");
    if (ofp == NULL)
    {
        return 0;
    }

    fseek(ifp, 0, SEEK_END);
    len = ftell(ifp);
    fseek(ifp, 0, SEEK_SET);

    printf("Файл, который нужно распаковать (%d символов): %s\n", len, filename);
    printf("Распаковка...\n");

    fread(&flength, sizeof(long int), 1, ifp);
    fread(&f, sizeof(long int), 1, ifp);
    fseek(ifp, f, SEEK_SET);
    fread(&n, sizeof(long int), 1, ifp);
    for (i = 0; i < n; i++)
    {
        fread(&header[i].a, 1, 1, ifp);
        fread(&c, 1, 1, ifp);
        p = (long int)c;
        header[i].count = p;
        header[i].bits[0] = 0;
        if (p % 8 > 0)
        {
            m = p / 8 + 1;
        }
        else
        {
            m = p / 8;
        }

        for (j = 0; j < m; j++)
        {
            fread(&c, 1, 1, ifp);
            f = c;
            _itoa(f, buf, 2);
            f = strlen(buf);
            for (l = 8; l > f; l--)
            {
                strcat(header[i].bits, "0");
            }
            strcat(header[i].bits, buf);
        }
        header[i].bits[p] = 0;
    }

    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (strlen(header[i].bits) > strlen(header[j].bits))
            {
                tmp = header[i];
                header[i] = header[j];
                header[j] = tmp;
            }
        }
    }

    p = strlen(header[n - 1].bits);
    fseek(ifp, 8, SEEK_SET);
    m = 0;
    bx[0] = 0;

    while (1)
    {
        while (strlen(bx) < (unsigned int)p)
        {
            fread(&c, 1, 1, ifp);
            f = c;
            _itoa(f, buf, 2);
            f = strlen(buf);
            for (l = 8; l > f; l--)
            {
                strcat(bx, "0");
            }
            strcat(bx, buf);
        }
        for (i = 0; i < n; i++)
        {
            if (memcmp(header[i].bits, bx, header[i].count) == 0)
            {
                break;
            }
        }
        strcpy(bx, bx + header[i].count);
        c = header[i].a;
        fwrite(&c, 1, 1, ofp);
        m++;

        if (100 * m / flength > per)
        {
            printf("%d%%\n", per);
            per += 10;
        }
        if (m == flength)
        {
            break;
        }
    }
    printf("100%%\n");

    fclose(ifp);
    fclose(ofp);

    printf("Файл успешно распакован!\n");

    return 1;
}

void printInstructions()
{
    printf("*Инструкция по использованию*\n");
    printf("\n");
    printf("Используйте данную команду со следующими аргументами:\n");
    printf("./Huffman_Compress <Команда> <\"Путь к исходному файлу\"> <\"Путь сохранения обработанного файла\">\n");
    printf("\n");
    printf("Список команд:\n");
    printf("encode - Сжать файл\n");
    printf("decode - Распаковать файл\n");
}

int main(int argc, const char* argv[])
{
    setlocale(LC_ALL, "Rus");

    if (argc < 4)
    {
        printInstructions();
        return 1;
    }

    const char* command = argv[1];
    const char* input = argv[2];
    const char* output = argv[3];

    if (strcmp(command, "encode") == 0)
    {
        memset(&header, 0, sizeof(header));
        memset(&tmp, 0, sizeof(tmp));
        compress(input, output);
    }
    else if (strcmp(command, "decode") == 0)
    {
        memset(&header, 0, sizeof(header));
        memset(&tmp, 0, sizeof(tmp));
        uncompress(input, output);
    }
    else
    {
        printInstructions();
        system("pause");
        return 1; 
    }

    //memset(&header, 0, sizeof(header));
    //memset(&tmp, 0, sizeof(tmp));

    //compress("test.jpg", "test_compressed.zip");
    //uncompress("test_compressed.zip", "test decompressed.jpg");

    system("pause");

    return 0;
}