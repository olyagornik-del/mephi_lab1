#ifndef LAB_TYPEINFO_H
#define LAB_TYPEINFO_H

typedef void (*BinaryOperation) (const void *, const void *, void *);
typedef void (*UnaryOperation) (const void *, void *);
typedef double (*ToDouble)(const void*);
typedef void* (*Allocator)();
typedef const void* (*ConstantGetter) ();
typedef void (*Printer) (const void*);

typedef struct TypeInfo {
    size_t elementSize;
    BinaryOperation add;
    BinaryOperation multiply;
    UnaryOperation assign;
    UnaryOperation square;
    ToDouble ToDouble;
    Allocator alloc;
    ConstantGetter zero;
    Printer print;
} TypeInfo;

#endif //LAB_TYPEINFO_H