#include "stdafx.h"
#include "Interfaz.h"
#include "Suponer.h"


Interfaz::Interfaz(Sudoku* ps)
{
   m_ps = ps;
}

#define m_tablero          m_ps->m_tablero
void
Interfaz::print() {
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

void
Interfaz::printCur(int const fila, int const columna) {
   for (int i = 0; i < 9; i++) {
      if (!(i % 3))
         std::cout << HORIZONTAL_LINE;
      for (int k = 0; k < 9; k++) {
         if (i == fila && k == columna)
            std::cout << "| " << CURSOR << " ";
         else if (m_tablero[i][k].con)
            std::cout << "| " << m_tablero[i][k].con << " ";
         else // Contenido de la casilla es 0
            std::cout << "|   ";
         if (!((k + 1) % 3)) // Extra bar for the cuadrant
            std::cout << "|";
      }
      std::cout << "\n";
   }
   // Última línea
   std::cout << HORIZONTAL_LINE;
}

bool
Interfaz::pide(bool bPrint /*= true*/, bool bCursor /*= true*/) { //introducir por teclado el sudoku a resolver

   bool puesto = false;
   //Limpiamos el tablero por si hay alguno cargado
   m_ps->vacia();

   for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
         while (!puesto) {
            PRINT_POSITION(i, j);
            if (bPrint) {
               if (bCursor) // imprimir usando cursor
                  printCur(i, j);
               else         // imprimir sin cursor
                  print();
            }
            std::cin >> m_tablero[i][j].con;
            if (NOT_VALID(m_tablero[i][j]) || !m_ps->comprobar(i, j, m_tablero[i][j].con)) {
               std::cout << "Posición imposible." << "\n";
               return false;
            }
            else
               puesto = true;
         }
         puesto = false;
      }
   }
   return true;
}

void
Interfaz::Comentar(int nMessage, int nFila, int nColumna) {
   if (!m_ps->GetSSComent())
      return;
   switch (nMessage) {
   case DESCARTAR:
      std::cout << "En la fila " << nFila << ", columna " << nColumna << " solo puede ir " << CONTENT(nFila, nColumna);
      break;
   case DESCARTAR_IMP:
      // No es necesario imprimir el tablero en este caso
      std::cout << "En la fila " << nFila << ", columna " << nColumna << " no puede ir ningún número.\n";
      return;
      break;
   case ESTUDIA_CUAD:
      std::cout << "En el cuadrante " << LINE_COLUMN_CUADRANT(nFila, nColumna) << " el número " << CONTENT(nFila, nColumna) <<
         " sólo puede ir en la fila " << nFila << " y columna " << nColumna;
      break;
   case ESTUDIA_FILA:
      std::cout << "En la fila " << nFila << " el número " << CONTENT(nFila, nColumna) <<
         " sólo puede ir en la fila " << nFila << " y columna " << nColumna;
      break;
   case ESTUDIA_COL:
      std::cout << "En la columna " << nColumna << " el número " << CONTENT(nFila, nColumna) <<
         " sólo puede ir en la fila " << nFila << " y columna " << nColumna;
      break;
   }
   print();
}

void
Interfaz::Comentar(int nMessage, sup Suposicion) {
   if (!m_ps->GetSSComent())
      return;
   switch (nMessage) {
   case NUEVA_SUP:
      std::cout << "En la fila " << Suposicion.atacar.fila << ", columna " << Suposicion.atacar.columna << " solo pueden ir ";
      for (int i = 0; i < Suposicion.candidatos.size(); i++) {
         std::cout << Suposicion.candidatos[i];
         if (i == Suposicion.candidatos.size() - 2)
            std::cout << " y ";
         else if (i < Suposicion.candidatos.size() - 2)
            std::cout << ", ";
      }
      std::cout << "\nSupongo que va un " << Suposicion.candidatos[0] << ".";
      break;
   case DESCARTA_SUP:
      std::cout << "En la fila " << Suposicion.atacar.fila << ", columna " << Suposicion.atacar.columna << " solo pueden ir ";
      for (int i = 0; i < Suposicion.candidatos.size(); i++) {
         std::cout << Suposicion.candidatos[i];
         if (i == Suposicion.candidatos.size() - 2)
            std::cout << " y ";
         else if (i < Suposicion.candidatos.size() - 2)
            std::cout << ", ";
      }
      std::cout << ".\nNo puede ir ";
      for (int i = 0; i < Suposicion.indice; i++) {
         std::cout << Suposicion.candidatos[i];
         if (i == Suposicion.indice - 2)
            std::cout << " ni ";
         else if (i < Suposicion.indice - 2 && Suposicion.indice != 1)
            std::cout << ", ";
      }
      if (Suposicion.indice < Suposicion.candidatos.size())
         std::cout << "\nSupongo que va un " << Suposicion.candidatos[Suposicion.indice] << ".";
      break;
   }
   print();
}