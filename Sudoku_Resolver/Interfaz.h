#pragma once
#include "stdafx.h"
#include "Tipos.h"
#include <string>

class Interfaz {
   Sudoku*  m_ps;
   char     simbols[9] = {'1','2','3','4','5','6','7','8','9'};
   bool     bSimbols = false;
   int      iIndexSimbol = 0;
public:
   Interfaz(Sudoku* ps);
   
   void print();
   void printCur(int const fila, int const columna);

   bool pide(bool bPrint = true, bool bCursor = true);
   bool CharACasilla(std::string schar, int *contenido);
   bool InSimbols(char newChar);
   int  ToInt(char newChar);
   int  GetIndex(char newChar);
   char ToChar(int nint);
   void ReturnNumbers();

   void Comentar(int nMessage, int nFila, int nColumna);
   void Comentar(int nMessage, sup Suposicion);
};
