cmake_minimum_required(VERSION 4.0)
project(lab C)
include_directories(H_files)

set(CMAKE_C_STANDARD 11)

add_executable(lab main.c
        TypeInfoComplex.c
        H_files/TypeInfoComplex.h
        H_files/TypeInfo.h
        TypeInfoDouble.c
        H_files/TypeInfoDouble.h
        Vector.c
        H_files/Vector.h
)
add_executable(test_c tests/test.c
        TypeInfoComplex.c
        H_files/TypeInfoComplex.h
        H_files/TypeInfo.h
        TypeInfoDouble.c
        H_files/TypeInfoDouble.h
        Vector.c
        H_files/Vector.h
        tests/testVector.c
        tests/testVector.h
        tests/testTypeInfoDouble.h
        tests/testTypeInfoDouble.c
        tests/testTypeInfoComplex.h
        tests/textTypeInfoComplex.c
)
