## README

## Pre-requisites

- Make installed
- GCC installed

## How to build

- Place yourself into the root directory

- Run make

```
make
```

## Project structure

```
\---Project
    +
    |----Makefile
    |----README.md
    |----.gitignore
    |----compiled_file #The compiled file will
    |                   #be set here
    |
    |
    |--- src
    |    +
    |    |---main
    |    |      main.c
    |    |
    |    |---package1
    |    |      file1_1.c
    |    |      file1_1.h
    |    |
    |    |---packageN
    |    |      file1_N.c
    |    |      file1_N.h
    |
    |--- build
    |    +
    |    |---main
    |    |      main.d
    |    |      main.o
    |    |
    |    |---package1
    |    |      file1_1.d
    |    |      file1_1.o
    |    |
    |    |---packageN
    |    |      file1_N.d
    |    |      file1_N.o

```
