#include <stdio.h>
#include <stdlib.h>


#include "H_files/TypeInfo.h"

typedef struct {
    double Re;
    double Im;
} Complex;

void* CreateComplex(double re, double im) {
    Complex* c = (Complex*)malloc(sizeof(Complex));
    if (c == NULL) {
        fprintf(stderr, "CreateComplex: не удалось выделить память\n");
        return NULL;
    }
    c->Re = re;
    c->Im = im;
    return c;
}

static void addComplex (const void *arg1, const void *arg2, void *result) {
    const Complex* a1 = ((const Complex*)arg1);
    const Complex* a2 = ((const Complex*)arg2);
    Complex* res = ((Complex*)result);
    res->Re = a1->Re + a2->Re;
    res->Im = a1->Im + a2->Im;
}
// static для функции нужно чтобы к функции в конце концов обращались только через указатели,
// то есть если произойдёт прямой вызов addComplex из другого файла-абстракция и могли бы быть ошибки.

static void multiplyComplex (const void *arg1, const void *arg2, void *result){
    const Complex* a1 = ((const Complex*)arg1);
    const Complex* a2 = ((const Complex*)arg2);
    Complex* res = ((Complex*)result);
    res->Re = (a1->Re)*(a2->Re) - (a1->Im)*(a2->Im);
    res->Im = (a1->Re)*(a2->Im) + (a2->Re)*(a1->Im);
}

static void assignComplex (const void *source, void *dest) {
    // source - откуда, dest - destination - куда
    ((Complex*)dest)->Re =((const Complex*)source)->Re;
    ((Complex*)dest)->Im =((const Complex*)source)->Im;
}

static void sqComplex(const void *arg, void *result) {
    const Complex* a = ((const Complex*)arg);
    Complex* res = ((Complex*)result);
    res->Re = (a->Re)*(a->Re) + (a->Im)*(a->Im);
}

static double toDouble_Re_C(const void* arg) {
    return ((const Complex*)arg)->Re; // квадрат модуля сразу
}

static void* allocComplex () {
    Complex* newComplex = (Complex*)malloc(sizeof(Complex));
    return newComplex;
    //возвращается указатель на новую переменную типа комплекс
}

static const void* zeroComplex () {
    static const Complex zero = {0.0, 0.0};
    return &zero;
};
//static — она создаётся в памяти только один раз при первом вызове функции и живёт всё время программы.

static void printComplex (const void *arg) {
    const Complex* a = ((const Complex*)arg);
    if (a->Im != 0) printf("%.2f + (%.2f)*i", a->Re, a->Im );
    else printf("%.2f", a->Re);
}

static const TypeInfo* ComplexTypeInfo = NULL;

const TypeInfo* GetComplexTypeInfo () {
    if (ComplexTypeInfo == NULL) {
        TypeInfo* info = (TypeInfo*)malloc(sizeof(TypeInfo));
        info->elementSize = sizeof(Complex);
        info->add = addComplex;
        info->multiply = multiplyComplex;
        info->assign = assignComplex;
        info->square = sqComplex;
        info->toDouble = toDouble_Re_C;
        info->alloc = allocComplex;
        info->zero = zeroComplex;
        info->print = printComplex;
        ComplexTypeInfo = info;
    }
    return ComplexTypeInfo;
}








