#!/bin/bash

echo "Cleaning up existing files..."
rm -f lex.yy.c parser.tab.c parser.tab.h transpiler.exe

# Check if all required files exist
if [ ! -f lexer.l ]; then
    echo "Error: lexer.l file not found!"
    exit 1
fi

if [ ! -f parser.y ]; then
    echo "Error: parser.y file not found!"
    exit 1
fi

# Run flex to generate lex.yy.c from lexer.l
echo "Running flex to generate lex.yy.c..."
flex lexer.l

# Check if lex.yy.c was generated
if [ ! -f lex.yy.c ]; then
    echo "Error: lex.yy.c was not generated!"
    exit 1
fi

# Run bison to generate parser.tab.c and parser.tab.h from parser.y
echo "Running bison to generate parser.tab.c and parser.tab.h..."
bison -d parser.y

# Check if parser.tab.c was generated
if [ ! -f parser.tab.c ]; then
    echo "Error: parser.tab.c was not generated!"
    exit 1
fi

# Compile everything with GCC
echo "Compiling with GCC..."
# gcc -o transpiler lex.yy.c parser.tab.c
gcc -g -o transpiler.exe lex.yy.c parser.tab.c 


# Check if the executable was generated
if [ ! -f transpiler ]; then
    echo "Error: transpiler executable was not created!"
    exit 1
fi

# Success message
echo "Build completed successfully! You can now run './transpiler'."
