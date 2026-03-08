#include <stdio.h>
#include <stdlib.h>

#include "H_files/TypeInfo.h"
#include "H_files/TypeInfoDouble.h"
#include "H_files/TypeInfoComplex.h"
#include "H_files/Vector.h"


// Очищает stdin до конца строки (после плохого ввода)
static void flushInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
static int scanInt(int *out) {
    if (scanf("%d", out) == 1) return 1;
    printf("  Ошибка: введите целое число!\n");
    flushInput();
    return 0;
}
static int scanDouble(double *out) {
    if (scanf("%lf", out) == 1) return 1;
    printf("  Ошибка: введите вещественное число!\n");
    flushInput();
    return 0;
}


//размерность вектора
size_t inputSize() {
    int n;
    printf("  Введите размерность вектора: ");
    while (!scanInt(&n)) {
        printf("  Введите размерность вектора: ");
    }
    if (n <= 0) {
        printf("  Хм, отрицательный вектор? Креативно, но нет. Ставлю 1.\n");
        return 1;
    }
    return (size_t)n;
}

void fillDoubleVector(Vector *v, size_t n) {
    const TypeInfo *ti = v->type_info;

    void **values = (void**)malloc(n * sizeof(void*));
    if (values == NULL) {
        fprintf(stderr, "  Не удалось выделить память для временного массива\n");
        return;
    }

    for (size_t i = 0; i < n; i++) {
        double val;
        printf("  Элемент [%zu]: ", i);
        while (!scanDouble(&val)) {
            printf("  Элемент [%zu]: ", i);
        }
        values[i] = CreateDouble(val);
    }

    FillVector(v, values, n);

    for (size_t i = 0; i < n; i++) free(values[i]);
    free(values);
}


void fillComplexVector(Vector *v, size_t n) {
    void **values = (void**)malloc(n * sizeof(void*));
    if (values == NULL) {
        fprintf(stderr, "  Не удалось выделить память для временного массива\n");
        return;
    }

    for (size_t i = 0; i < n; i++) {
        double re, im;

        printf("  Элемент [%zu] Re: ", i);
        while (!scanDouble(&re)) {
            printf("  Элемент [%zu] Re: ", i);
        }

        printf("  Элемент [%zu] Im: ", i);
        while (!scanDouble(&im)) {
            printf("  Элемент [%zu] Im: ", i);
        }
        values[i] = CreateComplex(re, im); // ← снаружи не знаем про struct Complex
    }

    FillVector(v, values, n);

    for (size_t i = 0; i < n; i++) free(values[i]);
    free(values);
}


void printBanner() {
    printf("\n");
    printf("   _   _           _              \n");
    printf("  | | | |         | |             \n");
    printf("  | | | | ___  ___| |_ ___  _ __ \n");
    printf("  | | | |/ _ \\/ __| __/ _ \\| '__|\n");
    printf("  \\ \\_/ /  __/ (__| || (_) | |   \n");
    printf("   \\___/ \\___|\\___|\\___|\\___/|_|  \n");
    printf("\n");
    printf("  double  ·  complex  ·  n-мерный \n");
    printf("  ──────────────────────────────── \n\n");
}

void printTypeMenu() {
    printf("\n  Выберите тип элементов:\n");
    printf("  1. Вещественные числа (double)\n");
    printf("  2. Комплексные числа\n");
    printf("  Тип: ");
}

void printMenu() {
    printf("\n  ┌──────────────────────────────────┐\n");
    printf("  │  1. Создать вектор A             │\n");
    printf("  │  2. Создать вектор B             │\n");
    printf("  │  3. Показать векторы             │\n");
    printf("  │  4. Сложить A + B                │\n");
    printf("  │  5. Скалярное произведение A · B │\n");
    printf("  │  6. Длина вектора A              │\n");
    printf("  │  7. Длина вектора B              │\n");
    printf("  │  0. Выход                        │\n");
    printf("  └──────────────────────────────────┘\n");
    printf("  Выберите: ");
}

int main(void) {
    printBanner();

    Vector *A = NULL;
    Vector *B = NULL;

    Vector *result = NULL;

    int choice;

    while (1) {
        printMenu();
        if (!scanInt(&choice)) continue;

        switch (choice) {

            case 1: { // создание вектора А
                // Если A уже существует — освобождаем старый
                if (A != NULL) {
                    DeleteVector(A);
                    printf(" Старый вектор A удалён, создаём новый!\n");
                }
                // сделать проверку на буквы(?)
                size_t n = inputSize();

                printTypeMenu();
                int typeChoice;
                while (!scanInt(&typeChoice)) {
                    printTypeMenu();
                }

                if (typeChoice == 1) {
                    A = CreateVector(n, GetDoubleTypeInfo());
                    if (A != NULL) fillDoubleVector(A, n);
                } else if (typeChoice == 2) {
                    A = CreateVector(n, GetComplexTypeInfo());
                    if (A != NULL) fillComplexVector(A, n);
                } else {
                    printf("  Неизвестный тип, попробуй ещё раз!\n");
                    break;
                }

                if (A != NULL) {
                    printf("  Вектор A создан: ");
                    PrintVector(A);
                    printf("\n");
                }
                break;
            } // case 1

            case 2: { // создание вектора В
                if (B != NULL) {
                    DeleteVector(B);
                    printf("  Старый вектор B удалён, создаём новый!\n");
                }

                size_t n = inputSize();

                printTypeMenu();
                int typeChoice;
                while (!scanInt(&typeChoice)) {
                    printTypeMenu();
                }

                if (typeChoice == 1) {
                    B = CreateVector(n, GetDoubleTypeInfo());
                    if (B != NULL) fillDoubleVector(B, n);
                } else if (typeChoice == 2) {
                    B = CreateVector(n, GetComplexTypeInfo());
                    if (B != NULL) fillComplexVector(B, n);
                } else {
                    printf("  Неизвестный тип!\n");
                    break;
                }

                if (B != NULL) {
                    printf("  Вектор B создан: ");
                    PrintVector(B);
                    printf("\n");
                }
                break;
            } // case 2

            case 3: { //принт векторов А и В
                printf("\n");

                // Проверяем что вектор существует перед печатью
                if (A != NULL) {
                    printf("  A = ");
                    PrintVector(A);
                    printf("\n");
                }
                else {
                    printf("  A = (не создан)\n");
                }

                if (B != NULL) {
                    printf("  B = ");
                    PrintVector(B);
                    printf("\n");
                }
                else {
                    printf("  B = (не создан)\n");
                }
                break;
            }//case 3

            case 4: { //сложение вектора
                // Проверяем что оба вектора существуют
                if (A == NULL || B == NULL) {
                    printf("  Сначала создайте оба вектора!\n");
                    break;
                }
                if (A->type_info != B->type_info) {
                    printf(" Нужно измениь тип данных у одного из векторов!\n");
                    break;
                }

                // Освобождаем старый result если был
                if (result != NULL) DeleteVector(result);

                result = CreateVector(A->size, A->type_info);

                if (result == NULL) break;

                addVector(A, B, result);

                printf("  A + B = ");
                PrintVector(result);
                printf("\n");
                break;
            } // case 4

            case 5: { // скалярное произведение
                if (A == NULL || B == NULL) {
                    printf("  Сначала создайте оба вектора!\n");
                    break;
                }

                const TypeInfo *ti = A->type_info;
                void *dotResult = ti->alloc();

                if (dotResult == NULL) {
                    fprintf(stderr, "  Не удалось выделить память под результат\n");
                    break;
                } // case 5

                DotVector(A, B, dotResult);

                printf("  A · B = ");
                ti->print(dotResult);
                printf("\n");

                free(dotResult);
                break;
            } // case 5

            case 6: { // длина вектора A
                if (A == NULL) {
                    printf("  Сначала создайте вектор A!\n");
                    break;
                }
                double lenA;
                NormVector(A, &lenA);
                printf("  |A| = %.2f\n", lenA);
                break;
            }//case 6

            case 7: { // длина вектора B
                if (B == NULL) {
                    printf("  Сначала создайте вектор B!\n");
                    break;
                }
                double lenB;
                NormVector(B, &lenB);
                printf("  |B| = %.2f\n", lenB);
                break;
            }//case 7

            case 0: { //выход
                printf("\n  Освобождаем память и уходим...\n");

                // ВАЖНО: всегда освобождаем память перед выходом
                // Иначе — утечка памяти (программа завершится, ОС очистит,
                // но это плохая привычка и на реальных серверах это проблема)
                if (A != NULL) DeleteVector(A);
                if (B != NULL) DeleteVector(B);
                if (result != NULL) DeleteVector(result);

                printf("  Пока! Хорошего дня! :)\n\n");
                return 0;
            } // case 0

            default:
                printf("  Неверная команда, попробуй цифру из меню!\n");
        } //switch
    } // while
} // функция



