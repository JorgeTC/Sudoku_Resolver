#include "stdafx.h"
#include "Casilla.h"
#include <vector>


casilla::casilla() {
   con = 0;
}

void
casilla::setFilaColumna(int f, int c)
{
   fila = f;
   columna = c;
   // We save the cuadrant
   cuadrar();
}

void
casilla::cuadrar() {
   // inicializa el cuadrante en base a su fila y columna
   cuad = LINE_COLUMN_CUADRANT(fila, columna);
}

CuadIndex::CuadIndex(int iCuad /*= 0*/) {
   // iCuad is the cuadrant to iterate
   index = 0;
   cuad = iCuad;
   fila = FIRST_LINE_CUADRANT(cuad);
   columna = FIRST_COLUMN_CUADRANT(cuad);
}

CuadIndex
CuadIndex::operator++() {
   index++;
   return *this;
}

CuadIndex
CuadIndex::operator=(int const& a) {
   index = a;
   return *this;
}

bool
CuadIndex::operator<(int const& a) {
   return index < a;
}

bool
CuadIndex::operator<=(int const& a) {
   return index <= a;
}