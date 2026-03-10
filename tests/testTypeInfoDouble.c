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

static void testGetDoubleTypeInfo_notNull () {
    assert(ti != NULL);
    printf("  [OK] testGetDoubleTypeInfo_notNull\n");
}

static void testSingleton () {
    const TypeInfo *ti1 = GetDoubleTypeInfo();
    const TypeInfo *ti2 = GetDoubleTypeInfo();
    assert(ti1 == ti2);
    printf("  [OK] testSingleton (один и тот же указатель при двух вызовах)\n");
}

static void testElementSize () {
    assert(ti->elementSize == sizeof(double));
    printf("  [OK] testElementSize\n");
}

static void testZero () {
    const void *z = ti->zero();
    assert(z != NULL);
    assert(fabs(*(const double*)z - 0.0) < EPS);
    printf("  [OK] testzero\n");
}

static void testAlloc () {
    void *obj = ti->alloc();
    assert(obj != NULL);
    free(obj);
    printf("  [OK] testAlloc\n");
}

static void testAssign () {
    void *src = makeDouble(3.14);
    void *dst = makeDouble(0);

    ti->assign(src, dst);
    assert(fabs(*(double*)src - 3.14) < EPS);

    free(src);
    free(dst);
    printf("  [OK] testAssign\n");
}

static void testAdd () {
    void *pos1 = makeDouble(2.0);
    void *pos2 = makeDouble(3.0);
    void *neg1 = makeDouble(-2);
    void *result = ti->alloc();

    // 2 + 3 = 5
    ti->add(pos1, pos2, result);
    assert(fabs(*(double*)result - 5.0) < EPS);

    //2 - 2  = 0
    ti->add(pos1, neg1, result);
    assert(fabs(*(double*)result - 0) < EPS);


    free(pos1);
    free(pos2);
    free(neg1);
    free(result);

    printf("  [OK] testAdd\n");
}

static void testMultiply () {
    void *num1 = makeDouble(4.0);
    void *num2 = makeDouble(2.5);
    void *result = ti->alloc();

    // 4 * 2.5 = 10
    ti->multiply(num1, num2, result);
    assert(fabs(*(double*)result - 10.0) < EPS);

    // 4 * 0 = 0
    ti->multiply(num1, ti->zero(), result);
    assert(fabs(*(double*)result - 0) < EPS);

    free(num1);
    free(num2);
    free(result);

    printf("  [OK] testMultiply\n");
}

static void testSquare () {
    void *pos = makeDouble(5.0);
    void *neg = makeDouble(-6.0);
    void *result = ti->alloc();

    // 5^2 == 25
    ti->square(pos, result);
    assert(fabs(*(double*)result - 25.0) < EPS);

    // (-6)^2 == 36
    ti->square(neg, result);
    assert(fabs(*(double*)result - 36) < EPS);

    // 0^2 = 0
    ti->square(ti->zero(), result);
    assert(fabs(*(double*)result - 0) < EPS);

    free(pos);
    free(neg);
    free(result);

    printf("  [OK] testSquare\n");
}

static void testToDouble() {
    void *a = makeDouble(7.77);

    double val = ti->toDouble(a);
    assert(fabs(val - 7.77) < EPS);

    free(a);

    printf("  [OK] testToDouble\n");
}




void testTypeInfoDoubleAll() {
    ti = GetDoubleTypeInfo();

    printf("=== Тесты TypeInfoDouble ===\n");
    testGetDoubleTypeInfo_notNull();
    testSingleton();
    testElementSize();
    testZero();
    testAlloc();
    testAssign();
    testAdd();
    testMultiply();
    testSquare();
    testToDouble();
    printf("=== Все тесты пройдены! ===\n");
}