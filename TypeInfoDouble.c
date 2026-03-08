#include <stdio.h>
#include <stdlib.h>

#include "H_files/TypeInfo.h"
#include "H_files/TypeInfoDouble.h"

typedef struct Double{
    double value;
} Double;

void* CreateDouble(double value) {
    Double* d = (Double*)malloc(sizeof(Double));
    if (d == NULL) {
        fprintf(stderr, "CreateDouble: не удалось выделить память\n");
        return NULL;
    }
    d->value = value;
    return d;
}

static void addDouble (const void *arg1, const void *arg2, void *result) {
    ((Double*)result)->value = (((const Double*)arg1)->value) + (((const Double*)arg2)->value);
}

static  void multiplyDouble (const void *arg1, const void *arg2, void *result) {
    ((Double*)result)->value = (((const Double*)arg1)->value)*((const Double*)arg2)->value;
}

static void assignDouble (const void *source, void *dest) {
    ((Double*)dest)->value = ((const Double*)source)->value;
}

static void sqDouble (const void *arg, void *res) {
    ((Double*)res)->value = (((const Double*)arg)->value)*(((const Double*)arg)->value);
}

static double toDoubleD(const void* arg) {
    return ((const Double*)arg)->value;
}

static void* allocDouble () {
    Double *newDouble = (Double*)malloc(sizeof(Double));
    return newDouble;
}

static const void* zeroDouble () {
    static const Double zero = {0};
    return &zero;
}

static void printDouble (const void *arg) {
    printf("%.2f", ((Double*)arg)->value);
}

static const TypeInfo* DoubleTypeInfo = NULL;

const TypeInfo* GetDoubleTypeInfo () {
    if (DoubleTypeInfo == NULL) {
        TypeInfo* info = (TypeInfo*)malloc(sizeof(TypeInfo));
        info->elementSize = sizeof(double);
        info->add = addDouble;
        info->multiply = multiplyDouble;
        info->assign = assignDouble;
        info->square = sqDouble;
        info->ToDouble = toDoubleD;
        info->alloc = allocDouble;
        info->zero = zeroDouble;
        info->print = printDouble;
        DoubleTypeInfo = info;
    }
    return DoubleTypeInfo;
}









