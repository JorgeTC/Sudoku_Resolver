#include "stdafx.h"
#include "Sudoku.h"
#include "Suponer.h"
#include "Casilla.h"
#include "Interfaz.h"
#include <vector>

bool
Sudoku::comprobar(int const fila, int const columna, int const n) {
   //comprueba si n es un buen número para introducir en la posicion fila columna
   if (n < 0 || n > 9) // comprobamos el rango de n
      return false;
   if (n == 0) // Siempre es posible vaciar una casilla
      return true;
   if (GetSSValid() && fila + columna != 16)
      return esResoluble();
   for (int k = 0; k < 9; k++) { //comprueba atendiendo a la fila
      if (CONTENT(fila, k) == n && k != columna)
         return false;
   }
   for (int k = 0; k < 9; k++) { //comprueba atediendo a la columna
      if (CONTENT(k, columna) == n && k != fila)
         return false;
   }
   for (CuadIndex k(LINE_COLUMN_CUADRANT(fila, columna)); k < 9; ++k) { // comprueba atendiendo al cuadrante
      if (CONTENT(k.Fila(), k.Columna()) == n && k.Fila() != fila && k.Columna() != columna) {
         return false;
      }
   }
   return true;
}

bool
Sudoku::comprobar(casilla const& place, int const n)
{
   return comprobar(place.fila, place.columna, n);
}

bool
Sudoku::esResoluble()
{
   Sudoku* temp;
   bool bOk;

   temp = new Sudoku(m_tablero);
   temp->SetSSComent(false); // buscamos una solución sin comentarios
   temp->SetSSRand(false);   // buscamos una solución. No importa cuál.
   bOk = temp->resolver();
   delete temp;
   return bOk;
}


int
Sudoku::descarte(int const fila, int const columna) {

   int candidato;

   switch (posibleNumeroEn(fila, columna, candidato)) {
   case ONLY_ONE_POSSIBLE:
      CONTENT(fila, columna) = candidato;
      return ONLY_ONE_POSSIBLE;
   case NOT_POSSIBLE_NUMBER: //La casilla está vacía y no se puede poner ningún número
      return NOT_POSSIBLE_NUMBER;
   default:
      return MORE_THAN_ONE_POSSIBLE_NUMBER;
   }
}

int
Sudoku::descarte(casilla const& place)
{
   return descarte(place.fila, place.columna);
}

int
Sudoku::posibleNumeroEn(int const fila, int const columna, int& candidato) {
   if (!EMPTY(m_tablero[fila][columna])) {
      // No tiene sentido comprobar los posibles números en una casilla no vacía
      return NOT_EMPTY_BOX;
   }

   int cuenta = 0;

   for (int i = 1; i <= 9 && cuenta < 2; i++) {
      // Si puedo poner más de un número, salgo del bucle
      if (comprobar(fila, columna, i)) {
         cuenta++;
         candidato = i;
      }
   }
   switch (cuenta) {
   case 0:
      return NOT_POSSIBLE_NUMBER;
   case 1:
      return ONLY_ONE_POSSIBLE;
   default:
      candidato = 0;
      return MORE_THAN_ONE_POSSIBLE_NUMBER;
   }
}

int
Sudoku::posiblenumeroEn(casilla const& place, int& candidato)
{
   return posibleNumeroEn(place.fila, place.columna, candidato);
}

int
Sudoku::descartar() {
   // Rellenar las casillas en las que solo pueda ir un número
   bool exito = false;
   bool puesto;

   do {
      puesto = false;
      for (int i = 0; i < 9; i++) {
         for (int j = 0; j < 9; j++) { // bucle doble para recorrer todo el tablero
            switch (descarte(i, j)) {
            case NOT_POSSIBLE_NUMBER: //cout << "Error al descartar.";
               Comentar( COMENT_CODE::DESCARTAR_IMP, i, j);
               return NOT_POSSIBLE_NUMBER;
               //caso en el que haya una casilla vacía que no acepta ningún número
            case ONLY_ONE_POSSIBLE:
               Comentar( COMENT_CODE::DESCARTAR, i, j);
               puesto = true;
            default:
               break;
            }
         }
      }
      exito = exito || puesto;
   } while (puesto);
   return (int)exito;
}

bool
Sudoku::pide(bool bPrint /*= true*/, bool bCursor /*= true*/) { //introducir por teclado el sudoku a resolver

   return m_If->pide(bPrint, bCursor);
}

void
Sudoku::printCur(int const fila, int const columna) {
   m_If->printCur(fila, columna);
}

void
Sudoku::print() {
   m_If->print();
}

bool
Sudoku::estudiaCuadrante(int const cuadrante) {
   std::vector<int> faltan = { 1,2,3,4,5,6,7,8,9 };
   casilla candidata;
   bool puesto = true;
   int n;
   int contador;
   bool exito = false;
   while (puesto) {
      puesto = false;
      for (CuadIndex k(cuadrante); k < 9; ++k) { // Actualizamos la lista faltan
         actualizaLista(faltan, m_tablero[k.Fila()][k.Columna()].con);
      }
      n = faltan.size();
      for (int i = 0; i < n; i++) {
         contador = 0;
         for (CuadIndex k(cuadrante); k < 9; ++k) {
            if (EMPTY(m_tablero[k.Fila()][k.Columna()]) &&
               comprobar(k.Fila(), k.Columna(), faltan[i])) {
               contador++;
               candidata.fila = k.Fila();
               candidata.columna = k.Columna();
            }
         }
         if (contador == 1) { // Si solo hemos encontrado una casilla donde puede ir faltan[i]
            CONTENT_IN(m_tablero, candidata) = faltan[i];
            puesto = true;
            exito = true;
            Comentar( COMENT_CODE::ESTUDIA_CUAD, candidata.fila, candidata.columna);
         }
      }
   }
   return exito;
}

bool
Sudoku::estudiaFila(int const fila) {
   std::vector<int> faltan = { 1,2,3,4,5,6,7,8,9 };
   std::vector<int>::iterator index;
   casilla candidata;
   bool puesto = true;
   int n;
   int contador;
   bool exito = false;
   while (puesto) {
      puesto = false;
      for (int i = 0; i < 9; i++) { // Actualizamos la lista faltan
         actualizaLista(faltan, m_tablero[fila][i].con);
      }
      n = faltan.size();
      for (int i = 0; i < n; i++) {
         contador = 0;
         for (int j = 0; j < 9; j++) {
            if (EMPTY(m_tablero[fila][j]) && comprobar(fila, j, faltan[i])) {
               contador++;
               candidata.columna = j;
            }
         }
         if (contador == 1) { // Si solo hemos encontrado una casilla donde puede ir faltan[i]
            m_tablero[fila][candidata.columna].con = faltan[i];
            puesto = true;
            exito = true;
            Comentar( COMENT_CODE::ESTUDIA_FILA, fila, candidata.columna);
         }
      }
   }
   return exito;
}

bool
Sudoku::estudiaColumna(int const columna) {
   std::vector<int> faltan = { 1,2,3,4,5,6,7,8,9 };
   casilla candidata;
   bool puesto = true;
   int n;
   int contador;
   int exito = false;

   while (puesto) {
      puesto = 0;
      for (int i = 0; i < 9; i++) {
         actualizaLista(faltan, m_tablero[i][columna].con);
      }
      n = faltan.size();
      for (int i = 0; i < n; i++) {
         contador = 0;
         for (int j = 0; j < 9; j++) {
            if (EMPTY(m_tablero[j][columna]) && comprobar(j, columna, faltan[i])) {
               contador++;
               candidata.fila = j;
            }
         }
         if (contador == 1) { // Si solo hemos encontrado una casilla donde puede ir faltan[i]
            m_tablero[candidata.fila][columna].con = faltan[i];
            puesto = true;
            exito = true;
            Comentar( COMENT_CODE::ESTUDIA_COL, candidata.fila, columna);
         }
      }
   }
   return exito;
}

void
Sudoku::actualizaLista(std::vector<int>& list, int objetivo) {
   // Eliminamos objetivo de list

   std::vector<int>::iterator index; //el índice de una posición en un vector tiene un tipo propio
   index = find(list.begin(), list.end(), objetivo);
   if (index != list.end())
      list.erase(index);
}

bool
Sudoku::estudiaTablero() {
   bool puesto = 1 ;
   bool exito = false ;

   while (puesto) {
      puesto = false;
      for (int k = 0; k < 9; k++) {
         puesto = puesto || estudiaCuadrante(k);
         puesto = puesto || estudiaFila(k);
         puesto = puesto || estudiaColumna(k);
      }
      exito = exito || puesto;
   }
   return exito;
}

void Sudoku::emptyInLine(int nLine, int &cont)
{
   emptyInLine(m_tablero, nLine, cont);
}

void
Sudoku::emptyInLine(casilla tablero[9][9], int nLine, int &cont){
   cont = 0;
   for (int i = 0; i < 9; i++){
      if (tablero[nLine][i].isEmpty()) {
         cont++;
      }
   }
}

void
Sudoku::emptyInCol(int nCol, int &cont){
   emptyInCol(m_tablero, nCol, cont);
}

void
Sudoku::emptyInCol(casilla tablero[9][9], int nCol, int &cont){
   cont = 0;
   for (int i = 0; i < 9; i++) {
      if (tablero[i][nCol].isEmpty()) {
         cont++;
      }
   }
}

void
Sudoku::emptyInCuad(int nCuad, int &cont) {
   emptyInCol(m_tablero, nCuad, cont);
}

void
Sudoku::emptyInCuad(casilla tablero[9][9], int nCuad, int &cont) {
   cont = 0;
   for (CuadIndex k(nCuad); k < 9; ++k) {
      if (tablero[k.Fila()][k.Columna()].isEmpty()) {
         cont++;
      }
   }
}

void
Sudoku::Comentar( COMENT_CODE nMessage, int nFila /*= 0*/, int nColumna /*= 0*/)
{
   m_If->Comentar(nMessage, nFila, nColumna);
}

void
Sudoku::Comentar( COMENT_CODE nMessage, sup Suposicion)
{
   m_If->Comentar(nMessage, Suposicion);
}
