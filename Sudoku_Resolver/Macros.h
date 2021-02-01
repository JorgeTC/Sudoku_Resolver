#pragma once
#include "stdafx.h"

#define SWAP(a, b) {auto aux = a; a = b, b = aux;}


#define EMPTY(cs)                      (cs.con == 0)
#define NOT_VALID(cs)                  (cs.con < 0 || cs.con > 9)

#define CONTENT_IN(tablero, cs)        tablero[cs.fila][cs.columna].con
#define CONTENT(line, col)             m_tablero[line][col].con
#define CONTENT_CS(cs)                 m_tablero[cs.fila][cs.columna].con

#define LAST_ELEMENT(v)                v[v.size() -1]
#define FIRST_ELEMENT(v)               v[0]
#define IS_EMPTY(v)                    (v.size() == 0)
#define _S(str)                        std::string(str)
#define GET_FILE(name)                 _S("Examples\\") + name + _S(".txt")

#define NOT_EMPTY_BOX                  -2
#define NOT_POSSIBLE_NUMBER            -1
#define ONLY_ONE_POSSIBLE              1
#define MORE_THAN_ONE_POSSIBLE_NUMBER  0
#define DONE                           2
#define IMPOSSIBLE                     3

#define MISSING_NUMBER(n)              (45-n)

#define FIRST_LINE_CUADRANT(cd)        (int) (floor(cd / 3) * 3)
#define FIRST_COLUMN_CUADRANT(cd)      (int) (floor(cd % 3) * 3)
#define LINE_COLUMN_CUADRANT(ln, cl)   (int) (floor(ln / 3) * 3 + floor(cl / 3))
#define PRINT_POSITION(i,j)            std::cout << "Fila " << i << " columna " << j << " cuadrante " << LINE_COLUMN_CUADRANT(i,j) << ": \n"
#define PRINT_POSITION_CS(cs)          PRINT_POSITION(cs.fila, cs.columna)

#define HORIZONTAL_LINE                "--------------------------------------\n"
#define CURSOR                         "*"

#define IMPOSSIBLE_SUDOKU              "Sudoku imposible\n"
