#include "stdafx.h"
#include <fstream>
#include <string>
#include "Matriz.h"
#include "Suponer.h"

void
Sudoku::print() {
   std::cout << "\n";
   for (int i = 0; i < 9; i++) {
      if (!(i % 3)) std::cout << HORIZONTAL_LINE;
      for (int k = 0; k < 9; k++) {
         if (m_tablero[i][k].con) {
            std::cout << "| " << m_tablero[i][k].con << " ";
         }
         else std::cout << "|   ";
         if (!((k + 1) % 3))
            std::cout << "|";
      }
      std::cout << "\n";
   }
   std::cout << HORIZONTAL_LINE;
}

bool
Sudoku::resolver()
{
   bool bPuesto;
   bool error;

   do {
      bPuesto = false;
      error = false;
      // Intentamos rellenar por descarte---------------------------------
      switch (descartar()) { //pone todos los números que puede por descarte
      case 0:
         //cout << "\nNo he podido poner ninguna casilla por descarte.";
         break;
      case NOT_POSSIBLE_NUMBER:
         if (IS_EMPTY(suposiciones)) {
            // si hay casillas mal puestas y no hay suposiciones, es que el sudoku introducido es imposible
            return false;
         }
         else {
            error = true;
         }
         break;
      default:
         bPuesto = true;
         break;
      }
      //Intentamos rellenar por estudio------------------------------
      if (!error && !terminado()) {
         bPuesto = bPuesto || estudiaTablero();
      }
      if (terminado()) {
         return true;
      }
      if (!bPuesto) {
         bPuesto = probar(error);
         if (!bPuesto)
            return false;
      }
   } while (bPuesto);
}

bool
Sudoku::GetSolution()
{
   if (!resolver()) {
      std::cout << IMPOSSIBLE_SUDOKU;
      return false;
   }
   else {
      print();
      cout << "\n¡Sudoku terminado!\n" <<
         suposiciones.size() << "suposiciones necesarias.";
      return true;
   }
}

Sudoku::Sudoku() {
   for (int j = 0; j < 9; j++) {
      for (int k = 0; k < 9; k++) {
         m_tablero[j][k].fila = j;
         m_tablero[j][k].columna = k;
         m_tablero[j][k].cuad = LINE_COLUMN_CUADRANT(j, k); //asigna a cada casilla su cuadrante correspondiente
         m_tablero[j][k].con = 0;
      }
   }
}

Sudoku::Sudoku(int tablero[9][9]){
   for (int j = 0; j < 9; j++) {
      for (int k = 0; k < 9; k++) {
         m_tablero[j][k].fila = j;
         m_tablero[j][k].columna = k;
         m_tablero[j][k].cuad = LINE_COLUMN_CUADRANT(j, k); //asigna a cada casilla su cuadrante correspondiente
      }
   }
   copiar(tablero);
}

Sudoku::Sudoku(casilla tablero[9][9]){
   for (int j = 0; j < 9; j++) {
      for (int k = 0; k < 9; k++) {
         m_tablero[j][k].fila = j;
         m_tablero[j][k].columna = k;
         m_tablero[j][k].cuad = LINE_COLUMN_CUADRANT(j, k); //asigna a cada casilla su cuadrante correspondiente
      }
   }
   copiar(tablero);
}

bool
Sudoku::load(std::string fileName)
{
   std::ifstream myFile(GET_FILE(fileName));
   if (!myFile)
      return false;
   for (int j = 0; j < 9; j++) {
      for (int k = 0; k < 9; k++) {
         myFile >> m_tablero[j][k].con;
      }
   }
   myFile.close();
   return true;
}

bool
Sudoku::terminado() {
   // Comprueba que todas las casillas estén llenas.
   // TODO: comprobar que su contenido sea correcto
   for (int j = 0; j < 9; j++) {
      for (int k = 0; k < 9; k++) {
         if (m_tablero[j][k].isEmpty())
            return false;
      }
   }
   return true;
}

void
Sudoku::vacia(){
   // Deja todos los elementos a 0

   for (int k = 0; k < 9; k++) {
      for (int m = 0; m < 9; m++) {
         m_tablero[k][m].con = 0;
      }
   }
}

void
Sudoku::copiar(int tablero[9][9]){
   for (int k = 0; k < 9; k++) { //carga un tablero con los enteros de una matriz
      for (int m = 0; m < 9; m++) {
         m_tablero[k][m].con = tablero[k][m];
      }
   }
}

void
Sudoku::copiar(casilla tablero[9][9]){
   for (int k = 0; k < 9; k++) { //carga un tablero con los enteros de una matriz
      for (int m = 0; m < 9; m++) {
         m_tablero[k][m].con = tablero[k][m].con;
      }
   }
}

void
copiar(casilla tablero[9][9], int ejemplo[9][9]) {
   for (int k = 0; k < 9; k++) { //carga un tablero con los enteros de una matriz
      for (int m = 0; m < 9; m++) {
         tablero[k][m].con = ejemplo[k][m];
      }
   }
}

void copiar(int ejemplo[9][9], casilla tablero[9][9]) {
   for (int k = 0; k < 9; k++) { //los números de un tablero se guardan en una matriz
      for (int m = 0; m < 9; m++) {
         // solo nos interesa el contenido de las casillas
         ejemplo[k][m] = tablero[k][m].con;
      }
   }
}