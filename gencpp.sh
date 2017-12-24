#!/bin/bash

SMALL=$(echo $*)
BIG=$(echo $SMALL | tr a-z A-Z)

HPP='#ifndef __MYCLASS_HPP__\n
#define __MYCLASS_HPP__\n
\n
class MyClass\n
{\n
public:\n
    MyClass();\n
    ~MyClass();\n
\n
private:\n
    MyClass(MyClass &&) = default;\n
    MyClass(const MyClass &) = default;\n
    MyClass &operator=(MyClass &&) = default;\n
    MyClass &operator=(const MyClass &) = default;\n
    \n
};\n
\n
#endif\n
\n'

CPP="#include \"MyClass.hpp\"\n
\n
MyClass::MyClass()\n
{\n
}\n
\n
MyClass::~MyClass()\n
{\n
}\n"

echo --------------
echo $HPP | sed "s|\\\\n|\n|g" | sed "s|^ ||g" | sed "s|MYCLASS|$BIG|g" | sed "s|MyClass|$SMALL|g" >> $*.hpp
echo --------------
echo $CPP | sed "s|\\\\n|\n|g" | sed "s|^ ||g" | sed "s|MYCLASS|$BIG|g" | sed "s|MyClass|$SMALL|g" >> $*.cpp