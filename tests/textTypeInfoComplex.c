#include <stdlib.h> //size_t, malloc, free
#include <stdio.h> //printf,
#include <assert.h> // assert
#include <math.h> // fabs (norm)
#include "../H_files/TypeInfoComplex.h"

#include "testTypeInfoComplex.h"

#define EPS 1e-9

typedef struct {
    double Re;
    double Im;
} ComplexTest;

static const TypeInfo *ti = NULL;

static void* makeComplex(double re, double im) {
    return CreateComplex(re, im);  // просто делегируем
}

static double getRe(const void *obj) { return ((const ComplexTest*)obj)->Re; }
static double getIm(const void *obj) { return ((const ComplexTest*)obj)->Im; }


static void testGetComplexTypeInfo_notNull () {
    assert(ti != NULL);
    printf("  [OK] testGetComplexTypeInfo_notNull\n");
}

static void testSingleton () {
    const TypeInfo *ti1 = GetComplexTypeInfo();
    const TypeInfo *ti2 = GetComplexTypeInfo();
    assert(ti1 == ti2);
    printf("  [OK] testSingleton (один и тот же указатель при двух вызовах)\n");
}

static void testElementSize () {
    assert(ti->elementSize == sizeof(ComplexTest));
    printf("  [OK] testElementSize\n");
}

static void testZero() {
    const void *z = ti->zero();
    assert(z != NULL);
    assert(fabs(getRe(z) - 0.0) < EPS);
    assert(fabs(getIm(z) - 0.0) < EPS);
    printf("  [OK] testZero\n");
}

static void testAlloc () {
    void *obj = ti->alloc();
    assert(obj != NULL);
    printf("  [OK] testAlloc\n");

    free(obj);
}

static void testAssign () {
    void *src = makeComplex(3.14, 3.7);
    void *dst = ti->alloc();

    ti->assign(src, dst);

    assert(fabs(getRe(dst) - 3.14) < EPS);
    assert(fabs(getIm(dst) - 3.7) < EPS);
    printf("  [OK] testAssign\n");

    free(src);
    free(dst);
}

static void testAdd () {
    void *pos1 = makeComplex(2.0, 4);
    void *pos2 = makeComplex(3.0, 5);
    void *neg1 = makeComplex(-2, -4);
    void *result = ti->alloc();

    //сложение положительных
    ti->add(pos1, pos2, result);
    assert(fabs(getRe(result) - 5.0) < EPS);
    assert(fabs(getIm(result) - 9.0) < EPS);

    //сложение полож + нег
    ti->add(pos1, neg1, result);
    assert(fabs(*(double*)result - 0) < EPS);


    free(pos1);
    free(pos2);
    free(neg1);
    free(result);

    printf("  [OK] testAdd\n");
}

static void testMultiply () {
    void *a = makeComplex(1, 2);
    void *b = makeComplex(3, 4);
    void *c = makeComplex(0.0, 2.0);
    void *d = makeComplex(0.0, 3.0);
    void *result = ti->alloc();

    //(1+2i) * (3+4i) == (-5+10i)
    ti->multiply(a, b, result);
    assert(fabs(getRe(result) - (-5.0)) < EPS);
    assert(fabs(getIm(result) - 10.0) < EPS);

    //(1+2i) * 0.0 == 0.0
    ti->multiply(a, ti->zero(), result);
    assert(fabs(*(double*)result - 0) < EPS);

    //2i) * (3i) = -6
    ti->multiply(c, d, result);
    assert(fabs(getRe(result) - (-6.0)) < EPS);
    assert(fabs(getIm(result) - 0.0) < EPS);

    free(a);
    free(b);
    free(c);
    free(d);
    free(result);

    printf("  [OK] testMultiply\n");
}

static void testSquare () {
    void *a = makeComplex(3.0, 4.0);
    void *result = ti->alloc();

    //|3+4i|^2 == 25
    ti->square(a, result);
    assert(fabs(getRe(result) - 25.0) < EPS);

    // |0|^2 == 0
    ti->square(ti->zero(), result);
    assert(fabs(getRe(result) - 0.0) < EPS);

    // |5+0i|^2 == 25
    void *b = makeComplex(5.0, 0.0);
    ti->square(b, result);
    assert(fabs(getRe(result) - 25.0) < EPS);

    free(a);
    free(b);
    free(result);

    printf("  [OK] testSquare\n");
}

static void testToDouble() {
    void *a = makeComplex(7.5, 99.0);
    void *b = makeComplex(0.0, 5.0);

    assert(fabs(ti->toDouble(a) - 7.5) < EPS);

    // Re == 0
    assert(fabs(ti->toDouble(b) - 0.0) < EPS);

    free(a);
    free(b);

    printf("  [OK] testToDouble\n");
}




void testTypeInfoComplexAll() {
    ti = GetComplexTypeInfo();

    printf("=== Тесты TypeInfoComplex ===\n");
    testGetComplexTypeInfo_notNull();
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
