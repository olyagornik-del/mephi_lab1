#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "H_files/TypeInfo.h"
#include "H_files/Vector.h"

// вспомогательная функция — адрес i-го элемента
static void* getElement(const Vector* v, size_t i) {
    return (char*)v->data + i * v->type_info->elementSize;
    // char* для побайтовой арифметики указателей
}

Vector* CreateVector (size_t size, const TypeInfo *type_info) { //скорее всего попросят передать выделение памяти пользователю
    if (size == 0 || type_info == NULL) {
        fprintf(stderr, "размер 0 или нет информации о типе\n");
        return NULL;
    }


    Vector *v = (Vector*)malloc(sizeof(Vector));
    if (v==NULL) {
        fprintf(stderr, "Вектор: Не удалось выделить память :(\n");
        free(v);
        return NULL;
    }

    v->size = size;
    v->type_info = type_info;

    v->data = (void**)malloc(size * type_info->elementSize);
    if (v->data == NULL) {
        fprintf(stderr, "Массив: 'эх, снова не вышло!' не удалось выделить память :(\n");
        free(v);
        return NULL;
    }

    const void* zero = type_info->zero(); //чтобы потом все координаты 0 сделать
    for (size_t i = 0; i < size; i++ ) {
        type_info->assign(zero, getElement(v, i));
    }
    return v;
}

void DeleteVector (Vector *v) {
    if (v == NULL) return;
    free(v->data);
    free(v);
}

void FillVector (Vector *v, void **values, size_t values_size) {
    if (v == NULL || values == NULL) {
        fprintf(stderr, "Заполнение: зачем тут пустота.. хм...\n");
        return;
    }

    if (v->size != values_size) {
        fprintf(stderr, "Заполнение вектора: не совпадают размеры вектора, %zu, и данных, %zu!\n", v->size, values_size);
        return;
    }
    const TypeInfo* ti = v->type_info;

    for (size_t i = 0; i < values_size; i++) {
        if (values[i] == NULL) {
            fprintf(stderr, "Заполнение вектора: среди данных есть указ на пустоту\n");
            for (size_t j = 0; j < i; j++) (ti->assign(ti->zero(), getElement(v, j)));
            return;
        }
        ti->assign(values[i], getElement(v, i));
        // копируем значение ПО указателю в вектор
    }
}

void AddVector (const Vector *v1, const Vector *v2, Vector *v_res) {
    if (v1 == NULL || v2 == NULL || v_res == NULL) {
        fprintf(stderr, "Сложение: зачем дали NULL?\n");
        return;
    }
    if ( v1->type_info != v2->type_info || v2->type_info != v_res->type_info ) {
        fprintf(stderr, "Сложение: мне нужен совпадающий тип(\n");
        return;
    }
    const TypeInfo *ti = v1->type_info;
    if (v1->size != v2->size || v2->size != v_res->size) {
        fprintf(stderr, "Сложение: вектора разной размерности\n");
        return;
    }

    for (size_t i = 0; i < v1->size; i++) {
        ti->add(getElement(v1, i), getElement(v2, i), getElement(v_res, i));
    }
}

void DotVector (const Vector *v1, const Vector *v2, void *result) {
    if (v1 == NULL || v2 == NULL || result == NULL) {
        fprintf(stderr, "Скалярное умножение: зачем дали NULL?\n");
        return;
    }
    if (v1->type_info != v2->type_info) {
        fprintf(stderr, "Скалярное умножение: у векторов не совпадают типы данных\n");
        return;
    }
    const TypeInfo *typeinfo = v1->type_info;
    if (v1->size != v2->size) {
        fprintf(stderr, "Скалярное умножение: у векторов не совпадают размеры\n");
        return;
    }

    typeinfo->assign(typeinfo->zero(), result);
    void *mul_res = typeinfo->alloc();

    for (size_t i = 0; i < v1->size; i++ ) {
        typeinfo->multiply(getElement(v1, i), getElement(v2, i), mul_res);
        typeinfo->add(result, mul_res, result);
    }
    free(mul_res);
}

void NormVector (const Vector *v, double *result) {
    if (v == NULL || result == NULL) {
        fprintf(stderr, "VectorLength: NULL аргумент\n");
        return;
    }

    void* sq = v->type_info->alloc();
    double sum = 0.0;

    for (size_t i = 0; i < v->size; i++) {
        v->type_info->square(getElement(v, i), sq);
        sum += v->type_info->toDouble(sq);
    }

    free(sq);
    *result = sqrt(sum);
}

void PrintVector (const Vector *v) {
    if (v == NULL) {
        fprintf(stderr, "Принт: Зачем нул?\n");
        return;
    }
    printf("[");
    for (size_t i = 0; i < v->size; i++) {
        v->type_info->print(getElement(v, i));
        if (i < (v->size - 1)) printf(", ");
    }
    printf("]");
}









