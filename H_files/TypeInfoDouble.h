#ifndef LAB_TYPEINFODOUBLE_H
#define LAB_TYPEINFODOUBLE_H

#include "TypeInfo.h"

typedef struct Double Double;

void* CreateDouble(double value);

const TypeInfo *GetDoubleTypeInfo ();

#endif //LAB_TYPEINFODOUBLE_H