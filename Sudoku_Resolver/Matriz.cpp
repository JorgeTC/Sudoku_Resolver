#include "stdafx.h"
#include <fstream>
#include <string>
#include "Matriz.h"
#include "Suponer.h"
#include "Interfaz.h"



bool
Sudoku::resolver()
{
   bool bPuesto;
   bool error;

   do {
      error = false;
      switch ( razonar(&error) ) {
         case IMPOSSIBLE:
            return false;
         case DONE:
            return true;
         default:
            break;
      }
      bPuesto = probar(error);
      if (!bPuesto)
         return false;
   } while (bPuesto);
}

int
Sudoku::razonar(bool *error) {

   bool bPuesto = false;

   do {
      // Intentamos rellenar por descarte---------------------------------
      switch ( descartar() ) { //pone todos los n�meros que puede por descarte
         case 0:
            // No he podido poner ninguna casilla por descarte.
            bPuesto = false;
            break;
         case NOT_POSSIBLE_NUMBER:
            if ( IS_EMPTY( suposiciones ) ) {
               // si hay casillas mal puestas y no hay suposiciones, es que el sudoku introducido es imposible
               return IMPOSSIBLE;
            }
            else {
               *error = true;
            }
            break;
         default:
            bPuesto = true;
            break;
      }
      //Intentamos rellenar por estudio------------------------------
      if ( !error && !terminado() ) {
         bPuesto = bPuesto || estudiaTablero();
      }
      if ( terminado() ) {
         return DONE;
      }
   } while ( bPuesto );

   return 0; // Caso general
}

bool
Sudoku::GetSolution()
{
   if (!resolver()) {
      nSolucionable = SIN_SOLUCION;
      std::cout << IMPOSSIBLE_SUDOKU;
      return false;
   }
   else { 
      print();
      cout << "\n�Sudoku terminado!\n" <<
         suposiciones.size() << " suposiciones necesarias.";
      CuantasSoluciones();
      if (nSolucionable == SOL_NO_UNICA)
         std::cout << "\nSoluci�n no �nica.";
      else if (nSolucionable == UNA_SOLUCION)
         std::cout << "\n�nica soluci�n.";
      return true;
   }
}

int
Sudoku::CuantasSoluciones() {

   Sudoku* aux;
   casilla atacar;
   int     candidato;

   // Asumimos que hemos llegado a solucionar el sudoku
   for (int i = 0; i < suposiciones.size(); i++) {
      // Seleccionamos la casilla que acepta varios candidatos
      atacar = suposiciones[i].atacar;
      for (int j = 0; j < suposiciones[i].candidatos.size(); j++) {
         // Recorremos la lista de candidatos
         candidato = suposiciones[i].candidatos[j];

         // No nos interesa si la elecci�n que se ha hecho es resoluble
         if (candidato != CONTENT_CS(atacar)) {
            // Genero un nuevo Sudoku con el estado anterior a la suposici�n
            aux = new Sudoku(suposiciones[i].estado);
            // Introduzco el nuevo candidato
            aux->CONTENT_CS(atacar) = candidato;
            if (aux->esResoluble()) {
               // Me espero que no sea resoluble. Si s� lo es, he encontrado otra soluci�n.
               nSolucionable = SOL_NO_UNICA;
               return SOL_NO_UNICA;
            }
            delete aux;
         }
      }
   }
   nSolucionable = UNA_SOLUCION;
   return UNA_SOLUCION;
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
   // Inicilizo la interfaz
   m_If = new Interfaz(this);
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
   // Inicilizo la interfaz
   m_If = new Interfaz(this);
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
   // Inicilizo la interfaz
   m_If = new Interfaz(this);
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
   // Comprueba que todas las casillas est�n llenas.
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
   for (int k = 0; k < 9; k++) { //los n�meros de un tablero se guardan en una matriz
      for (int m = 0; m < 9; m++) {
         // solo nos interesa el contenido de las casillas
         ejemplo[k][m] = tablero[k][m].con;
      }
   }
}