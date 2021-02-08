#pragma once
#include "stdafx.h"
#include "Macros.h"
#include <cmath>

class casilla {
public:
   int con;       // contenido de la casilla
   int fila;      // indice de su fila
   int columna;   // indice de su columna
   int cuad;      // indice de su cuadrante
   // constructor
   casilla();
   void setFilaColumna(int f, int c);
   void cuadrar();
   inline bool isEmpty() { return con == 0; };
};

class CuadIndex : public casilla {
   // Iterator to scroll a given cuadrant
   int index;
public:
   // Constructor
   CuadIndex(int iCuad = 0);

   // operators
   CuadIndex operator++();
   CuadIndex operator=  (int const& a);
   bool      operator < (int const& a);
   bool      operator <=(int const& a);

   // output interface
   inline int Fila() { return (int)fila + floor(index / 3); };
   inline int Columna() { return (int)columna + index % 3; };
   inline int Index() { return index; };
};

