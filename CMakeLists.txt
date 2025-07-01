cmake_minimum_required(VERSION 3.15)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
project(parser LANGUAGES C)

add_executable(compiler include/data.h
                      include/decl.h
                      include/defs.h
                      src/main.c
                      src/scan.c 
                      src/tree.c
                      src/expr.c 
                      src/gen.c 
                      src/cg.c
                      src/stmt.c
                      src/misc.c
                      src/sym.c
                      src/decl.c
                      )
