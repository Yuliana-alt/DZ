#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#define N 100 

const char* students_path = "students.csv";

typedef struct student_t {
    char id[10]; // номер зачётной книжки
    char surname[20]; // фамилия
    char name[20]; // имя
    char patronumic[20]; // отчество
    char faculty[10]; // факультет
    char speciality[100]; // специальность
} student_t;



// считывание полей структуры
student_t read_student() {
    student_t student; // создаём структуру
    int j = 0;
    char str[N];
    char n_speciality[N];
    // считываем поля
    printf("Введите номер зачетной книжки: ");
    scanf("%s", student.id); // считываем номер зачётной книжки

    printf("Введите фамилию: ");
    scanf("%s", student.surname); // считываем фамилия

    printf("Введите имя: ");
    scanf("%s", student.name); // считываем имя

    printf("Введите отчество: ");
    scanf("%s", student.patronumic); // считываем отчество

    printf("Введите факультет: ");
    scanf("%s", student.faculty); // считываем факультет

    printf("Введите специальность: "); // считываем специальность
    str[0] = getchar();
    str[j] = getchar();
    while (str[j] != '\n') {
        j++;
        str[j] = getchar();
    }
    str[j] = '\0';
    j++;
    strncpy(n_speciality, str, j);
    strncpy(student.speciality, n_speciality, j);
    j = 0;
    return student; // возвращаем считанную структуру
}



// вывод структуры
void print_student(student_t student) {
    printf("| %s ", student.id);
    printf("| %s ", student.surname);
    printf("| %s ", student.name);
    printf("| %s ", student.patronumic);
    printf("| %s ", student.faculty);
    printf("| %s ", student.speciality);
    printf("|\n");
}



// получение индекса студента по номеру зачётной книжки
int get_index_by_id(student_t* students, int n, char* id) {
    // ищем книгу по номеру
    for (int i = 0; i < n; i++)
        if (!strcmp(id, students[i].id))

            return i;

    return -1; // не нашли
}

// добавление новой записи
student_t* add_student(student_t* students, int* n) {
    student_t student = read_student(); // считываем запись

    if (get_index_by_id(students, *n, student.id) != -1) { // если студент с таким номером уже есть
        printf("студент с таким номером уже есть\n"); // сообщаем об этом
        return students; // возвращаем исходный массив
    }

    printf("Информация о новом студенте добавлена\n");
    students[(*n)++] = student; // записываем книгу в массив

    return students; // возвращаем обновлённый массив
}

// удаление студента по номеру
void remove_student_by_id(student_t* students, int* n) {

    char id[10];
    printf("Введите номер зачетной книжки: ");
    scanf("%s", id); // считываем поле для поиска

    int i = get_index_by_id(students, *n, id); // ищем студента по номеру

    // если не нашли
    if (i == -1) {
        printf("Нет студента с таким номером '%s'\n", id); // сообщаем об этом
        return; // и выходим
    }

    (*n)--; // уменьшаем размер

    // сдвигаем книги влево ???????????????????
    for (; i < *n; i++)
        students[i] = students[i + 1];
    printf("Студент удален\n");
}

// редактирование записи
void edit_students(student_t* students, int n) {

    char id[10];
    printf("Введите номер зачетной книжки: ");
    scanf("%s", id);

    int index = get_index_by_id(students, n, id); // ищем студента

    if (index == -1) {
        printf("Нет студента с таким номером\n");
        return;
    }

    printf("Введите новую информацию\n");
    students[index] = read_student(); // считываем новые данные
}

// поиск записей по заданному полю
void find_students_by_field(student_t* students, int n) {

    char id[16];
    printf("Введите номер зачетной книжки: ");
    scanf("%s", id);

    int index = get_index_by_id(students, n, id); // ищем студента

    if (index == -1) {
        printf("Нет студента с таким номером\n");
        return;
    }


    print_student(students[index]); // выводим студента

}

// сохранение записей в текстовый файл
void save_students(student_t* students, int n) {
    FILE* f = fopen(students_path, "w"); // открываем файл на запись

    for (int i = 0; i < n; i++) {
        fprintf(f, "%s;", students[i].id);
        fprintf(f, "%s;", students[i].surname);
        fprintf(f, "%s;", students[i].name);
        fprintf(f, "%s;", students[i].patronumic);
        fprintf(f, "%s;", students[i].faculty);
        fprintf(f, "%s", students[i].speciality);

        if (i < n - 1)
            fprintf(f, "\n");
    }

    fclose(f); // закрываем файл
}

// загрузка записей из текстового файла
student_t* load_students(int* n) {
    FILE* f = fopen(students_path, "r"); // открываем файл на чтение

    if (!f) { // если не удалось открыть файл
        *n = 0; // обнуляем количество
        return NULL; // возвращаем пустой указатель
    }
    int m = 0;
    while (!feof(f)) {
        if (fgetc(f) == '\n')
            m++;
    }
    m++;
    student_t* students = (student_t*)malloc(m * sizeof(student_t)); // выделяем память под один элемент
    *n = 0; // изначально элементов нет

    while (!feof(f)) {
        fscanf(f, "%[^;]%*c", students[*n].id);
        fscanf(f, "%[^;]%*c", students[*n].surname);
        fscanf(f, "%[^;]%*c", students[*n].name);
        fscanf(f, "%[^;]%*c", students[*n].patronumic);
        fscanf(f, "%[^;]%*c", students[*n].faculty);
        fscanf(f, "%[^\n]", students[*n].speciality);

        *n = *n + 1; // увеличиваем размер
    }

    fclose(f); // закрываем файл
    return students; // возвращаем считанный массив
}
void students_menu() {
    int n; // количество элементов в массиве
    student_t* students = load_students(&n); // массив структур
    int item; // выбираемый пункт меню

    do {
        printf("Выберите пункт меню\n");
        printf("1. Добавить студента\n");
        printf("2. Удалить студента\n");
        printf("3. Редактировать информацию о студенте\n");
        printf("4. Посмотреть информацию о студенте\n");
        printf("5. Выход\n");
        printf(">");
        scanf("%d%*c", &item); // считываем пункт меню

        // обрабатываем некорректный ввод пункта меню
        while (item < 1 || item > 5) {
            printf("Произошла ошибка, попробуйте еще раз: ");
            scanf("%d%*c", &item); // считываем пункт меню ещё раз
        }

        if (item == 1) {
            students = add_student(students, &n); // добавляем новую запись
        }
        if (item == 2) {
            remove_student_by_id(students, &n);
        }
        if (item == 3) {
            edit_students(students, n);  // редактируем запись
        }
        if (item == 4) {
            find_students_by_field(students, n);
        }

        printf("\n");
    } while (item != 5); // повторяем, пока не решим выйти

    save_students(students, n); // сохраняем студентов в файл
    free(students); // освобождаем динамическую память
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Rus");
    char mode[N]; // режим работы

    {
        students_menu();
    }

        return 0;
    }

