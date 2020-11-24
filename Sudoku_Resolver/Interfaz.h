#pragma once
#include "stdafx.h"
#include "Tipos.h"

class Interfaz {
   Sudoku* m_ps;
public:
   Interfaz(Sudoku* ps);
   
   void print();
   void printCur(int const fila, int const columna);

   bool pide(bool bPrint = true, bool bCursor = true);

   void Comentar(int nMessage, int nFila, int nColumna);
   void Comentar(int nMessage, sup Suposicion);
};
