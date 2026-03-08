#include <stdlib.h> //size_t, malloc, free
#include <stdio.h> //printf,
#include <assert.h> // assert
#include <math.h> // fabs (norm)
#include "../H_files/TypeInfoDouble.h"

#include "testTypeInfoDouble.h"

#define EPS 1e-9

static const TypeInfo *ti = NULL;

static void* makeDouble(double v) {
    void *obj = ti->alloc();
    *(double*)obj = v;
    return obj;
}

static void test_GetDoubleTypeInfo_notNull () {
    assert(ti != NULL);
    printf("  [OK] GetDoubleTypeInfo возвращает не NULL\n");
}

static void test_singleton () {
    const TypeInfo *ti1 = GetDoubleTypeInfo();
    const TypeInfo *ti2 = GetDoubleTypeInfo();
    assert(ti1 == ti2);
    printf("  [OK] GetDoubleTypeInfo: один и тот же указатель при двух вызовах\n");
}

static void test_elementSize () {
    assert(ti->elementSize == sizeof(double));
    printf("  [OK] elementSize == sizeof(double)\n");
}

static void test_zero () {
    const void *z = ti->zero();
    assert(z != NULL);
    assert(fabs(*(const double*)z - 0.0) < EPS);
    printf("  [OK] zero() == 0.0\n");
}

static void test_alloc () {
    void *obj = ti->alloc();
    assert(obj != NULL);
    free(obj);
    printf("  [OK] alloc() не вернул NULL\n");
}

static void test_assign () {
    void *src = makeDouble(3.14);
    void *dst = ti->alloc();

    ti->assign(src, dst);

    assert(fabs(*(double*)dst - 3.14) < EPS);

    free(src);
    free(dst);
    printf("  [OK] assign: значение скопировано верно\n");
}

static void test_add () {
    void *pos1 = makeDouble(2.0);
    void *pos2 = makeDouble(3.0);
    void *neg1 = makeDouble(-2);
    void *result = ti->alloc();

    //сложение положительных
    ti->add(pos1, pos2, result);
    assert(fabs(*(double*)result - 5.0) < EPS);
    printf("  [OK] add: 2.0 + 3.0 == 5.0\n");

    //сложение полож + нег
    ti->add(pos1, neg1, result);
    assert(fabs(*(double*)result - 0) < EPS);
    printf("  [OK] add: 2.0 - 2.0 == 0.0\n");


    free(pos1);
    free(pos2);
    free(neg1);
    free(result);
}

static void test_multiply () {
    void *a = makeDouble(4.0);
    void *b = makeDouble(2.5);
    void *result = ti->alloc();

    ti->multiply(a, b, result);
    assert(fabs(*(double*)result - 10.0) < EPS);
    printf("  [OK] multiply: 4.0 * 2.5 == 10.0\n");

    ti->multiply(a, ti->zero(), result);
    assert(fabs(*(double*)result - 0) < EPS);
    printf("  [OK] multiply: 4.0 * 0.0 == 0.0\n");

    free(a);
    free(b);
    free(result);
}

static void test_square () {
    void *pos = makeDouble(5.0);
    void *neg = makeDouble(-6.0);
    void *result = ti->alloc();

    ti->square(pos, result);
    assert(fabs(*(double*)result - 25.0) < EPS);
    printf("  [OK] square: 5.0^2 == 25.0\n");

    ti->square(neg, result);
    assert(fabs(*(double*)result - 36) < EPS);
    printf("  [OK] square: (-6.0)^2 == 36.0\n");

    ti->square(ti->zero(), result);
    assert(fabs(*(double*)result - 0) < EPS);

    free(pos);
    free(neg);
    free(result);
}

static void test_ToDouble() {
    void *a = makeDouble(7.77);

    double val = ti->ToDouble(a);
    assert(fabs(val - 7.77) < EPS);
    printf("  [OK] ToDouble: возвращает верное значение\n");

    free(a);
}




void testTypeInfoDoubleAll() {
    ti = GetDoubleTypeInfo();

    printf("=== Тесты TypeInfoDouble ===\n");
    test_GetDoubleTypeInfo_notNull();
    test_singleton();
    test_elementSize();
    test_zero();
    test_alloc();
    test_assign();
    test_add();
    test_multiply();
    test_square();
    test_ToDouble();
    printf("=== Все тесты пройдены! ===\n");
}