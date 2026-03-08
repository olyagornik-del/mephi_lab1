#ifndef LAB_VECTOR_H
#define LAB_VECTOR_H

#include "TypeInfo.h"

typedef struct Vector{
    size_t size;
    void* data; //массив указателей на неизвестный тип , на буфер а не на массив
    const TypeInfo *type_info;
} Vector;

Vector* CreateVector (size_t size, const TypeInfo *type_info);
void DeleteVector (Vector *v);
void FillVector (Vector *v, void **values, size_t values_size);
void addVector (const Vector *v1, const Vector *v2, Vector *v_res);
void DotVector (const Vector *v1, const Vector *v2, void *result);
void NormVector (const Vector* v, double* result);
void PrintVector (const Vector *v);

#endif //LAB_VECTOR_H