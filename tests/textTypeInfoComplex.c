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


static void test_GetComplexTypeInfo_notNull () {
    assert(ti != NULL);
    printf("  [OK] GetComplexTypeInfo возвращает не NULL\n");
}

static void test_singleton () {
    const TypeInfo *ti1 = GetComplexTypeInfo();
    const TypeInfo *ti2 = GetComplexTypeInfo();
    assert(ti1 == ti2);
    printf("  [OK] GetComplexTypeInfo: один и тот же указатель при двух вызовах\n");
}

static void test_elementSize () {
    assert(ti->elementSize == sizeof(ComplexTest));
    printf("  [OK] elementSize == sizeof(ComplexTest)\n");
}

static void test_zero() {
    const void *z = ti->zero();
    assert(z != NULL);
    assert(fabs(getRe(z) - 0.0) < EPS);
    assert(fabs(getIm(z) - 0.0) < EPS);
    printf("  [OK] zero() == (0.0 + 0.0i)\n");
}

static void test_alloc () {
    void *obj = ti->alloc();
    assert(obj != NULL);
    printf("  [OK] alloc() не вернул NULL\n");

    free(obj);
}

static void test_assign () {
    void *src = makeComplex(3.14, 3.7);
    void *dst = ti->alloc();

    ti->assign(src, dst);

    assert(fabs(getRe(dst) - 3.14) < EPS);
    assert(fabs(getIm(dst) - 3.7) < EPS);
    printf("  [OK] assign: значение скопировано верно\n");

    free(src);
    free(dst);
}

static void test_add () {
    void *pos1 = makeComplex(2.0, 4);
    void *pos2 = makeComplex(3.0, 5);
    void *neg1 = makeComplex(-2, -4);
    void *result = ti->alloc();

    //сложение положительных
    ti->add(pos1, pos2, result);
    assert(fabs(getRe(result) - 5.0) < EPS);
    assert(fabs(getIm(result) - 9.0) < EPS);
    printf("  [OK] add: (2.0 + 4i) + (3 + 5i) = 5 + 9i\n");

    //сложение полож + нег
    ti->add(pos1, neg1, result);
    assert(fabs(*(double*)result - 0) < EPS);
    printf("  [OK] add: (2.0 + 4i) - (2.0 + 4i) == 0.0\n");


    free(pos1);
    free(pos2);
    free(neg1);
    free(result);
}

static void test_multiply () {
    void *a = makeComplex(1, 2);
    void *b = makeComplex(3, 4);
    void *c = makeComplex(0.0, 2.0);
    void *d = makeComplex(0.0, 3.0);
    void *result = ti->alloc();

    ti->multiply(a, b, result);
    assert(fabs(getRe(result) - (-5.0)) < EPS);
    assert(fabs(getIm(result) - 10.0) < EPS);
    printf("  [OK] multiply: (1+2i) * (3+4i) == (-5+10i)\n");

    ti->multiply(a, ti->zero(), result);
    assert(fabs(*(double*)result - 0) < EPS);
    printf("  [OK] multiply: (1+2i) * 0.0 == 0.0\n");

    // умножение двух чисто мнимых: (2i) * (3i) = -6
    ti->multiply(c, d, result);
    assert(fabs(getRe(result) - (-6.0)) < EPS);
    assert(fabs(getIm(result) -   0.0)  < EPS);
    printf("  [OK] multiply: (2i) * (3i) == -6\n");

    free(a);
    free(b);
    free(c);
    free(d);
    free(result);
}

static void test_square () {
    void *a = makeComplex(3.0, 4.0);
    void *result = ti->alloc();

    ti->square(a, result);
    assert(fabs(getRe(result) - 25.0) < EPS);
    printf("  [OK] square: |3+4i|^2 == 25\n");

    // нулевое: 0^2 + 0^2 = 0
    ti->square(ti->zero(), result);
    assert(fabs(getRe(result) - 0.0) < EPS);
    printf("  [OK] square: |0|^2 == 0\n");

    // чисто вещественное: (5+0i) -> 25
    void *b = makeComplex(5.0, 0.0);
    ti->square(b, result);
    assert(fabs(getRe(result) - 25.0) < EPS);
    printf("  [OK] square: |5+0i|^2 == 25\n");

    free(a);
    free(b);
    free(result);
}

static void test_ToDouble() {
    void *a = makeComplex(7.5, 99.0);
    void *b = makeComplex(0.0, 5.0);

    assert(fabs(ti->ToDouble(a) - 7.5) < EPS);
    printf("  [OK] ToDouble: возвращает Re == 7.5\n");

    // Re == 0
    assert(fabs(ti->ToDouble(b) - 0.0) < EPS);
    printf("  [OK] ToDouble: Re == 0 при чисто мнимом числе\n");

    free(a);
    free(b);
}




void testTypeInfoComplexAll() {
    ti = GetComplexTypeInfo();

    printf("=== Тесты TypeInfoComplex ===\n");
    test_GetComplexTypeInfo_notNull();
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
