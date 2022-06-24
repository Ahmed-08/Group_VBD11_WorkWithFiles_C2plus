#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <conio.h>
#include <io.h>
#include <string>
#include <cstdio>

// создается класс телефонный справочник
class TelephoneDirectory
{
public:

    // конструктор с одним параметром
    TelephoneDirectory(const char* path) : path_{path}
    {}

    // метод добавляет абонента в справочник (в файл)
    void add(const char* data);

    // поиск абонента
    int search(const char* data);

    // удалить абонента
    void deleteAbonent(const char* data);

    // удалить все
    void deleteAll();

    // изменить данные
    void modification(const char* oldData, const char* newData);

    // сохранить абонента
    void saveAbonent(const char* data, const char* path);

    // сохранить все
    void saveAll(const char* path);

    // вывод на экран
    void print();

private:
    const char* path_;
};

void TelephoneDirectory::add(const char* data)
{
    FILE* file = fopen(this->path_, "a");
    if (!file)
    {
        std::cout << "ERROR!\n";
    }
    else
    {
        fputs(data, file);
        fputs("\n", file);
    }
    fclose(file);
}

int TelephoneDirectory::search(const char* data)
{
    FILE* file = fopen(this->path_, "r");

    if (!file)
    {
        std::cout << "ERROR!\n";
    }
    else
    {
        char s[100];
        int i{ 0 };
        while (!feof(file))
        {
            fgets(s, 100, file);
            s[strlen(s) - 1] = '\0';
            if (!strcmp(s, data))
                return i;         
            ++i;
        }
        fclose(file);
    }
    return -1;
}


void TelephoneDirectory::deleteAbonent(const char* data)
{
    // если такой абонент существует
    int i = this->search(data);
    if (i != -1)
    {
        FILE* file = fopen(this->path_, "r");
        if (!file)
        {
            std::cout << "ERROR!\n";
        }
        else
        {
            // записываем дескриптор файла в переменную
            int handle = _fileno(file);
            // получаем размер файла
            int sizeFile = _filelength(handle);
            // создаем буфер такого размера
            char* buffer = new char[sizeFile] {0};
            // создаем массив для добавления строки из файла
            char str[100];

            while (!feof(file))
            {
                // записываем строку в файл
                fgets(str, 100, file);
                str[strlen(str) - 1] = '\0';
                // если строка не совпадает с той, которую нужно удалить, то добавляем ее в буфер
                if (strcmp(str, data) && !feof(file))
                {
                    strcat(buffer, str);
                    strcat(buffer, "\n");
                }
            }
            // закрываем файл
            fclose(file);

            /* 
                открываем файл на запись
                исходный файл удалиться 
                и создастся новый с тем же именем
            */
            file = fopen(this->path_, "w");
            if (!file)
            {
                std::cout << "ERROR!\n";
            }
            else
            {
                // записываем в файл все, что добавленов в буфер
                fwrite(buffer, sizeof(char), strlen(buffer), file);
            }

            // закрываем файл
            fclose(file);
            // удаляем буфер
            delete[] buffer;
        }
    }
}


void TelephoneDirectory::deleteAll()
{
/*
    открываем файл с тем же именем на запись,
    исходный файл удалиться,
    создастся пустой файл
*/
    FILE* file = fopen(this->path_, "w");
    fclose(file);
}

void TelephoneDirectory::modification(const char* oldData, const char* newData)
{
    int i = this->search(oldData);
    if (i != -1)
    {
        FILE* file = fopen(this->path_, "r+");
        if (!file)
        {
            std::cout << "ERROR!\n";
        }
        else
        {
            int handle = _fileno(file);
            int sizeFile = _filelength(handle) - strlen(oldData) + strlen(newData);
            char* buffer = new char[sizeFile] {0};

            char s[100];
            int count{ 0 };
            while (!feof(file))
            {
                fgets(s, 100, file);
                if(count != i && !feof(file))
                {
                    strcat(buffer, s);
                }
                else if (count == i && !feof(file))
                {
                    strcat(buffer, newData);
                    strcat(buffer, "\n");
                }
                ++count;
            }
            fclose(file);

            file = fopen(this->path_, "w");
            if (!file)
            {
                std::cout << "ERROR!\n";
            }
            else
            {
                fwrite(buffer, sizeof(char), strlen(buffer), file);
            }
            fclose(file);

            delete[] buffer;
        }
    }
}

void TelephoneDirectory::saveAbonent(const char* data, const char* path)
{
    int i = this->search(data);
    if (i != -1)
    {
        FILE* file = fopen(this->path_, "r");
        if (!file)
        {
            std::cout << "ERROR!\n";
        }
        else
        {
            int count{ 0 };
            char s[100];
            while (!feof(file))
            {
                // если записалась нужная строка, то выходим из цикла
                fgets(s, 100, file);
                if (count == i)
                {
                    break;
                }
                ++count;
            }
            fclose(file);

            // перезаписываем абонента в другой файл
            file = fopen(path, "a");
            if (!file)
            {
                std::cout << "ERROR!\n";
            }
            else
            {
                fputs(s, file);
                fclose(file);
            }
        }

    }
}

void TelephoneDirectory::saveAll(const char* path)
{
    FILE* file = fopen(this->path_, "r");
    if (!file)
    {
        std::cout << "ERROR!\n";
    }
    else
    {
        int handle = _fileno(file);
        int sizeFile = _filelength(handle);
        char* buffer = new char[sizeFile] {0};
        // функция добавляет все в буфер
        fread(buffer, sizeof(char), sizeFile, file);
        fclose(file);

        file = fopen(path, "a");
        // все данные из буфера записываются в другой файл
        fwrite(buffer, sizeof(char), strlen(buffer), file);
        fclose(file);

        delete[] buffer;
    }
}

// вывод происходит в отсортированном виде
void TelephoneDirectory::print()
{
    FILE* file = fopen(this->path_, "r");
    if (!file)
    {
        std::cout << "ERROR!\n";
    }
    else
    {
        int handle = _fileno(file);
        int sizeFile = _filelength(handle);
        char* buffer = new char[sizeFile] {0};
        char s[100];
        int count{ 0 };
        while (!feof(file))
        {
            fgets(s, 100, file);
            if (!feof(file))
            {
                strcat(buffer, s);
            }
            ++count;
        }
        fclose(file);

        
        std::string str1 = buffer;
        std::string* str2 = new std::string[count];
        int pos{ 0 };
        int index{ 0 };
        while (count--)
        {
            // каждая строка разделяется сиволом \n
            
            int p = pos;
            pos = str1.find("\n", pos);
            ++pos;
            // находим подстростроку и записываем в массив string
            std::string s1 = str1.substr(p, pos - p);
            str2[index++] = s1;
        }

        // сортируем массив
        for (int i = 0; i < index; ++i)
        {
            for (int j = index - 1; j > i; --j)
            {
                if (str2[j] < str2[j - 1])
                {
                    std::swap(str2[j], str2[j - 1]);
                }
            }
        }

        // печатаем на экран
        for (int i = 0; i < index; ++i)
        {
            std::cout << str2[i];
        }

        // освобождаем память
        delete[] buffer;
        delete[] str2;
    }
}



int main()
{
    // тест
    const char* path = "G://C++//projects//VBD11//file.txt";

    TelephoneDirectory telDir(path);
    
    telDir.add("Vasiliy +79501753344");
    telDir.add("Petr +79782632547");
    telDir.add("Dmitriy +79905372217");
    telDir.add("Anna +79013711219");
    telDir.print();
    std::cout << std::endl;

    telDir.deleteAbonent("Petr +79782632547");
    telDir.print();
    std::cout << std::endl;
    
    telDir.modification("Anna +79013711219", "Petr +79782632547");
    telDir.print();

    telDir.saveAbonent("Vasiliy +79501753344", "G://C++//projects//VBD11//file1.txt");

    telDir.saveAll("G://C++//projects//VBD11//file2.txt");

    telDir.deleteAll();

    return 0;
}