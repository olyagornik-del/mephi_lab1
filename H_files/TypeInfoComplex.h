#ifndef LAB_TYPEINFOCOMPLEX_H
#define LAB_TYPEINFOCOMPLEX_H

#include "TypeInfo.h"

typedef struct Complex Complex;

void* CreateComplex(double re, double im); //это отдельная операция

const TypeInfo* GetComplexTypeInfo ();

#endif //LAB_TYPEINFOCOMPLEX_H