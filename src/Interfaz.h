#pragma once
#include "stdafx.h"
#include "Sudoku.h"
#include <string>


enum class COMENT_CODE
{
   DESCARTAR = 1,
   ESTUDIA_CUAD = 2,
   ESTUDIA_FILA = 3,
   ESTUDIA_COL = 4,
   NUEVA_SUP = 5,
   CUANTAS_SOLUCIONES = 6,
   DESCARTA_SUP = 7,
   DESCARTAR_IMP = 8,
   TRINIDAD = 9
};

class Trinidad;

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

   void Comentar( COMENT_CODE nMessage, int nFila, int nColumna);
   void Comentar( COMENT_CODE nMessage, sup Suposicion);
   void Comentar( COMENT_CODE nMessage, Trinidad tr );
};
