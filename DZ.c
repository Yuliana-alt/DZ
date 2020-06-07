#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#define N 100 

const char* students_path = "students.csv";

typedef struct student_t {
    char id[10];
    char surname[20]; 
    char name[20]; 
    char patronumic[20]; 
    char faculty[10]; 
    char speciality[100]; 
} student_t;


student_t read_student() {
    student_t student; 
    int j = 0;
    char str[N];
    char n_speciality[N];
    // считываем поля
    printf("Введите номер зачетной книжки: ");
    scanf("%s", student.id); 

    printf("Введите фамилию: ");
    scanf("%s", student.surname); 

    printf("Введите имя: ");
    scanf("%s", student.name); 

    printf("Введите отчество: ");
    scanf("%s", student.patronumic); 

    printf("Введите факультет: ");
    scanf("%s", student.faculty); 

    printf("Введите специальность: "); 
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
    return student; 
}

void print_student(student_t student) {
    printf("| %s ", student.id);
    printf("| %s ", student.surname);
    printf("| %s ", student.name);
    printf("| %s ", student.patronumic);
    printf("| %s ", student.faculty);
    printf("| %s ", student.speciality);
    printf("|\n");
}

int get_index_by_id(student_t* students, int n, char* id) {

    for (int i = 0; i < n; i++)
        if (!strcmp(id, students[i].id))

            return i;

    return -1; 
}


student_t* add_student(student_t* students, int* n) {
    student_t student = read_student(); 

    if (get_index_by_id(students, *n, student.id) != -1) { 
        printf("студент с таким номером уже есть\n");
        return students;
    }

    printf("Информация о новом студенте добавлена\n");
    students[(*n)++] = student; 

    return students; 
}


void remove_student_by_id(student_t* students, int* n) {

    char id[10];
    printf("Введите номер зачетной книжки: ");
    scanf("%s", id);

    int i = get_index_by_id(students, *n, id); 

        if (i == -1) {
        printf("Нет студента с таким номером '%s'\n", id); 
        return; 
    }

    (*n)--; 

    for (; i < *n; i++)
        students[i] = students[i + 1];
    printf("Студент удален\n");
}


void edit_students(student_t* students, int n) {

    char id[10];
    printf("Введите номер зачетной книжки: ");
    scanf("%s", id);

    int index = get_index_by_id(students, n, id); 

    if (index == -1) {
        printf("Нет студента с таким номером\n");
        return;
    }

    printf("Введите новую информацию\n");
    students[index] = read_student(); 
}


void find_students_by_field(student_t* students, int n) {

    char id[16];
    printf("Введите номер зачетной книжки: ");
    scanf("%s", id);

    int index = get_index_by_id(students, n, id); 

    if (index == -1) {
        printf("Нет студента с таким номером\n");
        return;
    }


    print_student(students[index]); 

}


void save_students(student_t* students, int n) {
    FILE* f = fopen(students_path, "w"); 

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

    fclose(f); 
}

student_t* load_students(int* n) {
    FILE* f = fopen(students_path, "r"); 

    if (!f) { 
        *n = 0; 
        return NULL; 
    }
    int m = 0;
    while (!feof(f)) {
        if (fgetc(f) == '\n')
            m++;
    }
    m++;
    student_t* students = (student_t*)malloc(m * sizeof(student_t)); 
    *n = 0; 

    while (!feof(f)) {
        fscanf(f, "%[^;]%*c", students[*n].id);
        fscanf(f, "%[^;]%*c", students[*n].surname);
        fscanf(f, "%[^;]%*c", students[*n].name);
        fscanf(f, "%[^;]%*c", students[*n].patronumic);
        fscanf(f, "%[^;]%*c", students[*n].faculty);
        fscanf(f, "%[^\n]", students[*n].speciality);

        *n = *n + 1; 
    }

    fclose(f);
    return students;
}
void students_menu() {
    int n;
    student_t* students = load_students(&n);
    int item; 

    do {
        printf("Выберите пункт меню\n");
        printf("1. Добавить студента\n");
        printf("2. Удалить студента\n");
        printf("3. Редактировать информацию о студенте\n");
        printf("4. Посмотреть информацию о студенте\n");
        printf("5. Выход\n");
        printf(">");
        scanf("%d%*c", &item); 

   
        while (item < 1 || item > 5) {
            printf("Произошла ошибка, попробуйте еще раз: ");
            scanf("%d%*c", &item); 
        }

        if (item == 1) {
            students = add_student(students, &n); 
        }
        if (item == 2) {
            remove_student_by_id(students, &n);
        }
        if (item == 3) {
            edit_students(students, n); 
        }
        if (item == 4) {
            find_students_by_field(students, n);
        }

        printf("\n");
    } while (item != 5);

    save_students(students, n);
    free(students);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Rus");
    char mode[N];

    {
        students_menu();
    }

        return 0;
    }

