#include <stdlib.h> //size_t, malloc, free
#include <stdio.h> //printf,
#include <assert.h> // assert
#include <math.h> // fabs (norm)

#include "testVector.h"

#include "../H_files/Vector.h"
#include "../H_files/TypeInfo.h"

typedef struct { double value; } MockNum;

static const void* mock_zero() {
    static const MockNum z = {0.0};
    return &z;
}

static void* mock_alloc() {
    return malloc(sizeof(MockNum));
}

static void mock_assign(const void* src, void* dst) {
    ((MockNum*)dst)->value = ((const MockNum*)src)->value;
}

static void mock_add(const void* a, const void* b, void* res) {
    ((MockNum*)res)->value = ((const MockNum*)a)->value + ((const MockNum*)b)->value;
}

static void mock_multiply(const void* a, const void* b, void* res) {
    ((MockNum*)res)->value = ((const MockNum*)a)->value * ((const MockNum*)b)->value;
}

static void mock_square(const void* a, void* res) {
    double val = ((const MockNum*)a)->value;
    ((MockNum*)res)->value = val * val;
}

static double mock_toDouble(const void* a) {
    return ((const MockNum*)a)->value;
}

static void mock_print(const void* a) {
    printf("%.2f", ((const MockNum*)a)->value);
}

// Вспомогательная функция: собираем TypeInfo из mock-функций
static TypeInfo mock_ti = {
    sizeof(MockNum),  // elementSize
    mock_add,         // add
    mock_multiply,    // multiply
    mock_assign,      // assign
    mock_square,      // square
    mock_toDouble,    // ToDouble
    mock_alloc,       // alloc
    mock_zero,        // zero
    mock_print        // print
};


static Vector* makeVector(size_t n, double* vals, TypeInfo* ti) {
    Vector* v = CreateVector(n, ti);
    for (size_t i = 0; i < n; i++) {
        MockNum m = {vals[i]};
        ti->assign(&m, (char*)v->data + i * ti->elementSize);
    }
    return v;
}

void testCreateVector () {
    // size invalid
    Vector *vector = CreateVector(0, &mock_ti);
    assert(vector == NULL);

    // NULL type info
    vector = CreateVector(1, NULL);
    assert(vector == NULL);

    // Too much size
    vector = CreateVector((size_t)-1, &mock_ti);
    assert(vector == NULL);

    // All valid
    vector = CreateVector(8, &mock_ti);
    assert(vector != NULL);
    assert(vector->size == 8);
    assert(vector->data != NULL);
    assert(vector->type_info == &mock_ti);
    for (size_t i=0; i < 8; i++) {
        MockNum *elem = (MockNum*)((char*)vector->data + i * mock_ti.elementSize);
        assert(elem->value == 0.0);
    }
    // Граничный случай: вектор размера 1
    vector = CreateVector(1, &mock_ti);
    assert(vector != NULL);
    assert(vector->size == 1);


    free(vector->data);
    free(vector);

    printf("  [OK] testCreateVector\n");
}

void testDeleteVector () {

    //vector NULL
    DeleteVector(NULL);

    //valid vector
    Vector *v  = CreateVector(3, &mock_ti);
    assert(v != NULL);
    DeleteVector(v);

    printf("  [OK] testDeleteVector\n");
}

void testGetElementVector() {
    Vector *v = CreateVector(5, &mock_ti);

    MockNum *element = GetElementVector(NULL, 0);
    assert(element == NULL);

    element = GetElementVector(v, -1);
    assert(element == NULL);

    element = GetElementVector(v, 2);
    assert(element->value == 0);

    DeleteVector(v);

    printf("  [OK] testGetElementVector\n");
}

void testSetElementVector() {
    double valsv1[] = {1.0, 2.0, 3.0};
    MockNum newVal = {3.5};

    Vector *v = makeVector(3, valsv1, &mock_ti);
    void *ptr = &newVal;
    MockNum *result = (MockNum*)GetElementVector(v, 2);

    //передаём пустой вектор, проверяем не изменился ли указатель на новый элемент
    SetElementVector(NULL, 2, ptr);
    assert(ptr == &newVal);

    //передаём пустой указ на новый, значение в векторе не должно поменяться
    SetElementVector(v, 2, NULL);
    result = (MockNum*)GetElementVector(v, 2);
    assert(fabs(result->value - 3) < 1e-9);

    //передаём нормально
    SetElementVector(v, 2, ptr);
    result = (MockNum*)GetElementVector(v, 2);
    assert(fabs(result->value - 3.5) < 1e-9);

    //проверим что остальные эл-ты не изменились
    MockNum *elem1 = (MockNum*)GetElementVector(v, 0);
    assert(fabs(elem1->value - 1) < 1e-9);
    MockNum *elem2 = (MockNum*)GetElementVector(v, 1);
    assert(fabs(elem2->value - 1) < 1e-9);

    DeleteVector(v);

    printf("  [OK] testSetElementVector\n");
}


void testFillVector () {
    void* wrongPtrs[2];
    MockNum tmp0 = {1.0}, tmp1 = {2.0};
    wrongPtrs[0] = &tmp0; wrongPtrs[1] = &tmp1;

    void* ptrs[3];
    MockNum m0={10.0}, m1={20.0}, m2={30.0};
    ptrs[0]=&m0; ptrs[1]=&m1; ptrs[2]=&m2;

    //NULL передаём дважды
    FillVector(NULL, NULL, 0);

    Vector *vector = CreateVector(3, &mock_ti);
    assert(vector != NULL);

    //val - NULL
    FillVector(vector, NULL, 3);

    //не совпал размер
    FillVector(vector, (void**)wrongPtrs, 2);

    // Проверяем что данные записались
    FillVector(vector, (void**)ptrs, 3);
    MockNum *e0 = (MockNum*)((char*)vector->data + 0 * mock_ti.elementSize);
    MockNum *e1 = (MockNum*)((char*)vector->data + 1 * mock_ti.elementSize);
    MockNum *e2 = (MockNum*)((char*)vector->data + 2 * mock_ti.elementSize);
    assert(e0->value == 10.0);
    assert(e1->value == 20.0);
    assert(e2->value == 30.0);

    DeleteVector(vector);
    printf("  [OK] testFillVector\n");
}

void testAddVector () {
    double valsv1[] = {1.0, 2.0, 3.0};
    double valsv2[] = {4.0, 5.0, 6.0};
    TypeInfo mock_ti2 = mock_ti;

    Vector *v1 = makeVector(3, valsv1, &mock_ti); //c маленькой
    Vector *v2 = makeVector(3, valsv2, &mock_ti);
    Vector *v0 = CreateVector(3, &mock_ti);
    Vector *smalV = CreateVector(1, &mock_ti);
    Vector *diffrentTIVector = CreateVector(3, &mock_ti2);

    assert((v1 != NULL) && (v2!= NULL) && (v0 != NULL) && (smalV != NULL) && (diffrentTIVector != NULL));


    //передаём нул
    AddVector(NULL, v2, v0);
    AddVector(v1, NULL, v0);
    AddVector(v1, v2, NULL);

    //передаём вектор с другим типом данных
    AddVector(diffrentTIVector, v2, v0);
    //assert(R)

    //передаём вектор другой размерноси
    AddVector(smalV, v2, v0);

    // Нормальное сложение: [1,2,3] + [4,5,6] = [5,7,9]
    AddVector(v1, v2, v0);

    MockNum* r0 = (MockNum*)((char*)v0->data + 0 * mock_ti.elementSize);
    MockNum* r1 = (MockNum*)((char*)v0->data + 1 * mock_ti.elementSize);
    MockNum* r2 = (MockNum*)((char*)v0->data + 2 * mock_ti.elementSize);
    assert(r0->value == 5.0);
    assert(r1->value == 7.0);
    assert(r2->value == 9.0);

    DeleteVector(v1);
    DeleteVector(v2);
    DeleteVector(v0);
    DeleteVector(smalV);
    DeleteVector(diffrentTIVector);

    printf("  [OK] testAddVector\n");
}

void testDotVector () {
    double valsA[] = {1.0, 2.0, 3.0};
    double valsB[] = {4.0, 5.0, 6.0};
    TypeInfo mock_ti2 = mock_ti;

    Vector *v1 = makeVector(3, valsA, &mock_ti);
    Vector *v2 = makeVector(3, valsB, &mock_ti);
    Vector *v0 = CreateVector(3, &mock_ti);
    MockNum R = {0.0};
    Vector *smalV = CreateVector(1, &mock_ti);
    Vector *diffrentTIVector = CreateVector(3, &mock_ti2);

    assert((v1 != NULL) && (v2!= NULL) && (smalV != NULL) && (diffrentTIVector != NULL));

    // один из указателей нул
    DotVector(NULL, v2, &R);
    DotVector(v1, NULL, &R);
    DotVector(v1, v2, NULL);

    //разные типы
    DotVector(v1, smalV, &R);

    // разные размеры
    DotVector(v1, diffrentTIVector, &R);

    // нормальный случай
    // [1,2,3] · [4,5,6] = 1*4 + 2*5 + 3*6 = 4+10+18 = 32
    DotVector(v1, v2, &R);
    assert(R.value == 32);

    //нулевой веткор
    DotVector(v0, v1, &R);
    assert(R.value == 0);

    DeleteVector(v1);
    DeleteVector(v2);
    DeleteVector(smalV);
    DeleteVector(diffrentTIVector);
    DeleteVector(v0);

    printf("  [OK] testDotVector\n");
}

void testNormVector () {
    double valsv1[] = {3.0, 4.0};
    double valsBasis[] = {1, 0, 0};

    Vector *v1 = makeVector(2, valsv1, &mock_ti);
    Vector *v0 = CreateVector(3, &mock_ti);
    Vector *vb = makeVector(3, valsBasis, &mock_ti);
    double res = {56.0};

    //Нул в аргументах
    NormVector(NULL, &res);
    assert(res == 56);
    NormVector(v1, NULL);

    //нулевой вектор
    NormVector(v0, &res);
    assert(fabs(res - 0) < 1e-9);

    //вектор с длинной 1
    NormVector(vb, &res);
    assert(fabs(res - 1) < 1e-9);

    //адекватный вектор
    NormVector(v1, &res);
    assert(fabs(res - 5) < 1e-9);

    DeleteVector(v1);
    DeleteVector(vb);
    DeleteVector(v0);

    printf("  [OK] testLengthVector\n");

}


void testVectorAll () {
    printf("=== Тесты Vector ===\n");
    testCreateVector();
    testDeleteVector();
    testGetElementVector();
    testFillVector();
    testAddVector();
    testDotVector();
    testNormVector();
    printf("=== Все тесты пройдены! ===\n");
}